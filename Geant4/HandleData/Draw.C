#include <iostream>
#include <string>

#include "TFile.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TAxis.h"
#include "TCanvas.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

void Draw()
{
    //Set the parameter

    // string fileName("Graphic.root");
    // auto file = new TFile(fileName.c_str(), "READ");
    // if (!file->IsOpen())
    // {
    //     cout << "Can't open " << fileName << endl;
    //     exit(1);
    // }
    // TH3D *data = nullptr;
    // TH1D *energyDistribute = new TH1D("Energy Distribute", "Energy Distribute", 100, 10000, 11200);
    // string nameOfEvent("Event_");
    // for (int i = 0; i < file->GetListOfKeys()->GetEntries(); i++)
    // {
    //     if (!(i % 100))
    //         cout << "Event number: " << i << endl;
    //     data = static_cast<TH3D *>(file->Get((nameOfEvent + std::to_string(i)).c_str()));
    //     energyDistribute->Fill(data->GetSum());
    //     delete data;
    // }
    // TCanvas *canvas = new TCanvas("Energy Distribute", "Energy Distribute", 5);
    // energyDistribute->Draw();
    // canvas->Print("Energy Distribute.pdf");

    
    //Set the parameter
    int numberOfEvent = 1;

    string fileName("Graphic.root");
    auto file = new TFile(fileName.c_str(), "READ");
    if (!file->IsOpen())
    {
        cout << "Can't open " << fileName << endl;
        exit(1);
    }
    TH3D *data = nullptr;
    string nameOfEvent("Event_");
    data = static_cast<TH3D *>(file->Get((nameOfEvent + std::to_string(numberOfEvent)).c_str()));
    if (!data)
    {
        cout << "Can't find " << nameOfEvent << numberOfEvent << endl;
        exit(1);
    }

    TH2D *layer2D = new TH2D(string("layer").c_str(),
                             string("layer").c_str(),
                             data->GetNbinsX(), data->GetXaxis()->GetBinLowEdge(1), data->GetXaxis()->GetBinUpEdge(data->GetNbinsX()),
                             data->GetNbinsY(), data->GetYaxis()->GetBinLowEdge(1), data->GetYaxis()->GetBinUpEdge(data->GetNbinsY()));
    double max_temp(0);
    TAxis *xAxis = data->GetXaxis();
    TAxis *yAxis = data->GetYaxis();
    TCanvas *canvas = new TCanvas("picture", "picture", 1);
    canvas->cd();
    for (int whichLayer = 1; whichLayer <= data->GetNbinsZ(); whichLayer++)
    {
        layer2D->SetNameTitle((string("layer") + std::to_string(whichLayer)).c_str(), (string("layer") + std::to_string(whichLayer)).c_str());
        for (int x_temp = 1; x_temp <= data->GetNbinsX(); x_temp++)
            for (int y_temp = 1; y_temp <= data->GetNbinsY(); y_temp++)
            {
                if (!data->GetBinContent(x_temp, y_temp, whichLayer))
                    continue;
                // if (data->GetBinContent(x_temp, y_temp, whichLayer) < 0.001)
                //     continue;
                layer2D->Fill(xAxis->GetBinCenter(x_temp), yAxis->GetBinCenter(y_temp), data->GetBinContent(x_temp, y_temp, whichLayer));
            }
        // max_temp = layer2D->GetSum();
        // for (int x_temp = 1; x_temp <= layer2D->GetNbinsX(); x_temp++)
        //     for (int y_temp = 1; y_temp <= layer2D->GetNbinsY(); y_temp++)
        //         layer2D->SetBinContent(x_temp,y_temp,layer2D->GetBinContent(x_temp,y_temp) / max_temp);
        layer2D->Draw("COLZ");
        if (whichLayer == 1)
            canvas->Print("Picture.pdf(");
        else if (whichLayer == data->GetNbinsZ())
            canvas->Print("Picture.pdf)");
        else
            canvas->Print("Picture.pdf");
        layer2D->Reset();
    }
    
}