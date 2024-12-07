#include "tmppage.h"
// #include "plotComponents/livplot.h"
#include "plotComponents/spectraplot.h"

// TODO:
// This is a temporary page used to test out how the plotting looks
// Add this to wizard stack to test out graphs.

TmpPage::TmpPage(QWidget *parent)
    : WizardPage{"TmpPage", parent}
{
    // QVariantMap v = {{{"/Users/hyuna/Downloads/L1879/CW/"
    //                    "CW_L1879SK050624-BOT-SM-10K-2p0mm-100um-167Hz1Vsens-TC(50ms)-1MOhm.txt",
    //                    "10"},
    //                   {"/Users/hyuna/Downloads/L1879/CW/"
    //                    "CW_L1879SK050624-BOT-SM-10K-2p0mm-100um-167Hz1Vsens-TC(50ms)-1MOhm_2.txt",
    //                    "20"}}};
    QVariantMap tmp = {{{"/Users/hyuna/Downloads/L1879/spectra/"
                         "New_FINAL_BOT_L1879_SMQCL_20K_3.54THz_700mA.txt",
                         "20"},
                        {"/Users/hyuna/Downloads/L1879/spectra/"
                         "New_FINAL_BOT_L1879_SMQCL_50K_3.54THz_700mA.txt",
                         "50"},
                        {"/Users/hyuna/Downloads/L1879/spectra/"
                         "New_BOT_L1879_SMQCL_30K_3.54THz_700mA.txt",
                         "30"},
                        {"/Users/hyuna/Downloads/L1879/spectra/"
                         "New_FINAL_BOT_L1879_SMQCL_50K_3.54THz_700mA.txt",
                         "50"}}};
    // QCustomPlotDataWrapper *data1 = new QCustomPlotDataWrapper("hi", v, "temperature", true);

    QCustomPlotDataWrapper *data = new QCustomPlotDataWrapper("hi2", tmp, "temperature", false);

    // addWidget(new LIVPlot(data1, 1, 2, this));
    addWidget(new SpectraPlotStacked(data, this));
    addWidget(new SpectraPlotSamePlot(data, this));
}
