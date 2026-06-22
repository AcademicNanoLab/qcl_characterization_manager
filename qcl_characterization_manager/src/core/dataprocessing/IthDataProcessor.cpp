/**
 * \file        IthDataProcessor.cpp
 * \brief       Processes LIV data to extract Ith (threshold current) and dynamic range,
 *              and provides polynomial and exponential fitting capabilities.
 * \author      Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "IthDataProcessor.h"
#include <cmath>
#include <algorithm>

/**
 * \brief Constructor that initializes and processes LIV data.
 * 
 * Immediately processes the input data using the provided threshold.
 * 
 * \param data Pointer to the LIVDataProcessor object containing the data.
 * \param threshold The normalized threshold value for Ith extraction.
 * \param parent Optional parent QObject.
 */
IthDataProcessor::IthDataProcessor(LIVDataProcessor *data, double threshold, QObject *parent)
    : QObject(parent)
{
    process(data, threshold);
}

/**
 * \brief Extracts Ith (threshold current) and dynamic range for each trace in the dataset.
 * 
 * For each temperature trace in the dataset, this function:
 * - Locates the current value at which the normalized output crosses the given threshold.
 * - Stores the threshold current (Ith) and temperature (T).
 * - Computes the dynamic range (I_max - I_min) above the threshold and stores it.
 * 
 * \param data Pointer to the LIVDataProcessor object.
 * \param threshold The threshold for output intensity used to define Ith.
 */
void IthDataProcessor::process(LIVDataProcessor *data, double threshold)
{
    const QList<QString>& valueList = data->getValueList();
    const QList<QVector<double>>& xList = data->getXList();
    const QList<QVector<double>>& y2List = data->getY2List();  // already normalized

    for (int idx = 0; idx < valueList.size(); ++idx)
    {
        const QVector<double>& I = xList[idx];
        const QVector<double>& L = y2List[idx];

        if (I.size() != L.size()) {
            qWarning() << "Skipping trace" << idx << ": mismatched I/L size";
            continue;
        }

        // Find threshold current (first point where L >= threshold)
        int thresholdIndex = -1;
        for (int j = 0; j < L.size(); ++j) {
            if (L[j] >= threshold) {
                thresholdIndex = j;
                break;
            }
        }

        if (thresholdIndex == -1) {
            // No point exceeded threshold — skip this trace
            continue;
        }

        double T_val = valueList[idx].toDouble();
        T.append(T_val);

        double Jth = I[thresholdIndex];
        Ith.append(Jth);

        // Compute dynamic range over all I[j] where L[j] >= threshold
        double I_min = std::numeric_limits<double>::max();
        double I_max = std::numeric_limits<double>::lowest();

        for (int j = 0; j < L.size(); ++j) {
            if (L[j] >= threshold) {
                I_min = std::min(I_min, I[j]);
                I_max = std::max(I_max, I[j]);
            }
        }

        double DR_val = I_max - I_min;
        DR.append(DR_val * 1000.0);  // Convert to mA
    }
}

/**
 * \brief Performs a polynomial least-squares fit of specified order.
 *
 * Uses the Vandermonde matrix and Gaussian elimination to solve for coefficients.
 * Results are stored internally and also returned through the \p coefficients argument.
 * 
 * \param x Input x-values (typically temperature).
 * \param y Input y-values (typically Ith).
 * \param order Degree of the polynomial.
 * \param coefficients Output vector for polynomial coefficients.
 * \return true if fit was successful, false otherwise.
 */
bool IthDataProcessor::polynomialFit(const QVector<double>& x, const QVector<double>& y, int order, QVector<double>& coefficients)
{
    int n = x.size();
    if (n <= order) return false;  // At least (order + 1) data points required for the fit

    // Construct the Vandermonde matrix X (size: n x (order + 1))
    QVector<QVector<double>> X(n, QVector<double>(order + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= order; j++) {
            X[i][j] = std::pow(x[i], j);  // Fill the matrix with powers of x[i]
        }
    }

    // Calculate XtX (transpose(X) * X) and XtY (transpose(X) * y)
    QVector<QVector<double>> XtX(order + 1, QVector<double>(order + 1, 0.0));
    QVector<double> XtY(order + 1, 0.0);

    for (int i = 0; i <= order; i++) {
        for (int j = 0; j <= order; j++) {
            for (int k = 0; k < n; k++) {
                XtX[i][j] += X[k][i] * X[k][j];  // Sum for each element of XtX
            }
        }

        // Calculate XtY
        for (int k = 0; k < n; k++) {
            XtY[i] += X[k][i] * y[k];  // Sum for each element of XtY
        }
    }

    // Use Gaussian elimination (or LU decomposition) to solve the system of linear equations
    // Augmented matrix XtX | XtY
    QVector<QVector<double>> augmentedMatrix(order + 1, QVector<double>(order + 2, 0.0));
    for (int i = 0; i <= order; i++) {
        for (int j = 0; j <= order; j++) {
            augmentedMatrix[i][j] = XtX[i][j];  // Copy XtX to augmented matrix
        }
        augmentedMatrix[i][order + 1] = XtY[i];  // Copy XtY to augmented matrix
    }

    // Gaussian elimination to solve the system
    for (int i = 0; i <= order; i++) {
        // Pivoting: Find the maximum element in the current column to avoid numerical instability
        int maxRow = i;
        for (int j = i + 1; j <= order; j++) {
            if (std::abs(augmentedMatrix[j][i]) > std::abs(augmentedMatrix[maxRow][i])) {
                maxRow = j;
            }
        }

        if (i != maxRow) {
            std::swap(augmentedMatrix[i], augmentedMatrix[maxRow]);  // Swap rows
        }

        // Eliminate below the pivot
        for (int j = i + 1; j <= order; j++) {
            double factor = augmentedMatrix[j][i] / augmentedMatrix[i][i];
            for (int k = i; k <= order + 1; k++) {
                augmentedMatrix[j][k] -= augmentedMatrix[i][k] * factor;
            }
        }
    }

    // Back substitution to solve for coefficients
    coefficients.resize(order + 1);
    for (int i = order; i >= 0; i--) {
        coefficients[i] = augmentedMatrix[i][order + 1] / augmentedMatrix[i][i];
        for (int j = i - 1; j >= 0; j--) {
            augmentedMatrix[j][order + 1] -= augmentedMatrix[j][i] * coefficients[i];
        }
    }

    // Store the coefficients as a class member
    polynomialCoefficients = coefficients;

    return true;
}

/**
 * \brief Performs an exponential fit of the form y = A * exp(Bx) + C₀.
 *
 * Uses linearization and least squares to extract A, B, and C₀.
 * Ensures all adjusted y-values are positive to enable logarithmic transformation.
 *
 * \param x Input x-values (e.g., temperature).
 * \param y Input y-values (e.g., Ith).
 * \param A Output coefficient for exponential term.
 * \param B Output exponent factor.
 * \param C0 Output offset term.
 * \return true if fit was successful, false otherwise.
 */
bool IthDataProcessor::exponentialFit(const QVector<double>& x, const QVector<double>& y, double& A, double& B, double& C0)
{
    int n = x.size();
    if (n < 2) return false;

    double y_min = *std::min_element(y.begin(), y.end());
    C0 = y_min;

    QVector<double> y_adj(n);
    for (int i = 0; i < n; i++) {
        y_adj[i] = y[i] - 0.99 * y_min;
    }

    bool validAdjustment = true;
    for (int i = 0; i < n; i++) {
        if (y_adj[i] <= 0) {
            validAdjustment = false;
            break;
        }
    }

    if (!validAdjustment) {
        return false;
    }

    QVector<double> log_y_adj(n);
    for (int i = 0; i < n; i++) {
        log_y_adj[i] = std::log(y_adj[i]);
    }

    double sumX = 0, sumY = 0, sumXX = 0, sumXY = 0;
    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += log_y_adj[i];
        sumXX += x[i] * x[i];
        sumXY += x[i] * log_y_adj[i];
    }

    double denominator = n * sumXX - sumX * sumX;
    if (denominator == 0) return false;

    B = (n * sumXY - sumX * sumY) / denominator;
    double lnA = (sumY - B * sumX) / n;
    A = std::exp(lnA);

    // Store the exponential fit parameters in the class members
    A_exp = A;
    B_exp = B;
    C0_exp = C0;

    return true;
}

/**
 * \brief Generates a linearly spaced vector of values.
 * 
 * \param start Start of the range.
 * \param end End of the range.
 * \param numPoints Number of points to generate.
 * \return Vector of evenly spaced values between start and end.
 */
QVector<double> IthDataProcessor::linspace(double start, double end, int numPoints)
{
    QVector<double> result;
    double step = (end - start) / (numPoints - 1);
    for (int i = 0; i < numPoints; ++i) {
        result.append(start + i * step);
    }
    return result;
}

/**
 * \brief Applies a polynomial fit to Ith vs. T and evaluates it at regular intervals.
 * 
 * \param numPoints Number of evaluation points in the fit range.
 * \param order Degree of the polynomial.
 * \return Pair of vectors: fitted temperatures and corresponding Ith values.
 */
std::pair<QVector<double>, QVector<double>> IthDataProcessor::applyPolynomialFit(int numPoints, int order)
{
    // Generate linearly spaced x values based on T_min and T_max
    QVector<double> T_fit = linspace(T.first(), T.last(), numPoints);

    QVector<double> coefficients;
    if (!polynomialFit(T, Ith, order, coefficients)) {
        return {T_fit, QVector<double>()}; // Return empty if fit fails
    }

    QVector<double> Ith_fit;
    for (int i = 0; i < T_fit.size(); ++i) {
        double y_val = 0.0;
        for (int j = 0; j < coefficients.size(); ++j) {
            y_val += coefficients[j] * std::pow(T_fit[i], j);
        }
        Ith_fit.append(y_val);
    }

    return {T_fit, Ith_fit}; // Return fitted T and Ith
}

/**
 * @brief Applies an exponential fit to the threshold current data.
 * 
 * Fits the threshold current \( I_{th} \) as a function of temperature \( T \)
 * using the model:
 * \f[
 *     I_{th}(T) = A \cdot \exp(B \cdot T) + C_0
 * \f]
 * where \( A \), \( B \), and \( C_0 \) are the fitted parameters.
 * 
 * This function generates a linearly spaced temperature vector with the specified
 * number of points and evaluates the fitted exponential model over this range.
 * 
 * If the fit fails, the function returns an empty vector for the fitted \( I_{th} \).
 * 
 * @param[in] numPoints Number of temperature points to generate for the fitted curve.
 * @return A pair of vectors: the fitted temperature values and the corresponding fitted \( I_{th} \) values.
 */
std::pair<QVector<double>, QVector<double>> IthDataProcessor::applyExponentialFit(int numPoints)
{
    // Generate linearly spaced x values based on T_min and T_max
    QVector<double> T_fit = linspace(T.first(), T.last(), numPoints);

    double A, B, C0;
    if (!exponentialFit(T, Ith, A, B, C0)) {
        return {T_fit, QVector<double>()}; // Return empty if fit fails
    }

    QVector<double> Ith_fit;
    for (int i = 0; i < T_fit.size(); ++i) {
        double y_val = A * std::exp(B * T_fit[i]) + C0;
        Ith_fit.append(y_val);
    }

    return {T_fit, Ith_fit}; // Return fitted T and Ith
}

/**
 * @brief Retrieves the parameters of the exponential fit.
 * 
 * This function returns the coefficients \( A \), \( B \), and \( C_0 \) from the exponential
 * fit of the form:
 * \f[
 *     I_{th}(T) = A \cdot \exp(B \cdot T) + C_0
 * \f]
 * 
 * @param[out] A Exponential coefficient A.
 * @param[out] B Exponential coefficient B.
 * @param[out] C0 Offset term C0.
 */
void IthDataProcessor::getExponentialFitParams(double &A, double &B, double &C0) const
{
    A = A_exp;
    B = B_exp;
    C0 = C0_exp;
}

/**
 * @brief Retrieves the polynomial fit coefficients.
 * 
 * Populates the provided vector with the coefficients of the polynomial fit to the
 * dynamic range (DR) data as a function of temperature. The coefficients are ordered
 * from lowest degree to highest, i.e.,:
 * \f[
 *     DR(T) = c_0 + c_1 T + c_2 T^2 + \ldots + c_n T^n
 * \f]
 * 
 * @param[out] coefficients Vector to be filled with polynomial coefficients.
 */
void IthDataProcessor::getPolynomialCoefficients(QVector<double>& coefficients) const
{
    coefficients = polynomialCoefficients;
}