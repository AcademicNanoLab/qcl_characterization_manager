/**
 * @file HelpDialog.cpp
 * @brief Implementation of HelpDialog class for displaying application help content.
 * 
 * The HelpDialog presents a read-only text area containing step-by-step instructions
 * for using the QCL Characterization Manager. It also includes a button to close the dialog.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "HelpDialog.h"
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>

/// @brief Constructs and initializes the HelpDialog UI elements.
HelpDialog::HelpDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("How to Use the App");
    resize(900, 700);  // Increased size for better readability

    QVBoxLayout *layout = new QVBoxLayout(this);

    QTextBrowser *textBrowser = new QTextBrowser(this);
    textBrowser->setOpenExternalLinks(true);  // Enable opening links in the default browser
    textBrowser->setHtml(R"(
        <h2>Welcome to <i>QCL Characterization Manager</i>!</h2>
        <p>This application helps you:</p>
        <ul>
            <li>Start a <b>guided wizard</b> to characterize QCL devices</li>
            <li><b>Process previously measured</b> characterization files</li>
            <li><b>Automatically export</b> device datasheets in LaTeX</li>
        </ul>

        <h3>Steps:</h3>
        <ol>
            <li><b>Click "Start Wizard"</b> to launch the device setup workflow.</li>
            <li>Fill in all required parameters and metadata.</li>
            <li>Use the visualization and export tools to complete the process.</li>
            <li>Optionally, reprocess existing datasets via <i>'Process customised files'</i>.</li>
        </ol>

        <p>
        The <b>Start Wizard</b> button allows you to select different types of files:<br>
        <ul>
            <li><i>LIVs</i>: three-column data (typically I-V-L)</li>
            <li><i>Spectra</i>: two-column data (typically Wavelength-Intensity)</li>
        </ul>
        These relate to pulsed and CW characterization. Each wizard page is <b>optional</b> — fill only what applies to your data.
        </p>

        <p>
        On the last wizard page, you can:
        <ul>
            <li>Click <b>"Generate Grace Images"</b> to create .agr (Grace), .jpeg, and .pdf figures</li>
            <li>Click <b>"Generate Datasheet"</b> to export your measurements in a LaTeX document</li>
        </ul>
        <b>Note:</b> Both actions are optional — don't forget to click <i>"Finish"</i> to finalize.
        </p>

        <h3>Improving Outputs</h3>
        <p>
        If you're unhappy with the wizard output (e.g., overlapping legends on figures, or some missing data in the datasheet), you can:
        <ul>
            <li>Exit the app</li>
            <li>Manually edit .agr files (e.g., using <a href='https://sourceforge.net/projects/qtgrace/'>qtgrace</a>)</li>
            <li>Adjust the .tex file as needed</li>
            <li>Restart the app and use <b>"Process Customised Files"</b> button to reprocess figures and regenerate the LaTeX document</li>
        </ul>
        </p>
    )");

    layout->addWidget(textBrowser);

    QPushButton *closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &HelpDialog::accept);
    layout->addWidget(closeBtn);
}
