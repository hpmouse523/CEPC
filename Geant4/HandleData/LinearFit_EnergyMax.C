#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cmath>

#include "TFile.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TAxis.h"
#include "TTree.h"
#include "TCanvas.h"

using std::cout;
using std::endl;
using std::vector;
using std::array;
using std::string;
using std::sqrt;

typedef array<double, 2> Point2D;
typedef array<double, 3> Position;
double Distance(Position a, Position b)
{
    return sqrt((a.at(0) - b.at(0)) * (a.at(0) - b.at(0)) + (a.at(1) - b.at(1)) * (a.at(1) - b.at(1)) + (a.at(2) - b.at(2)) * (a.at(2) - b.at(2)));
}

array<double, 2> Fit(vector<Point2D> pointSet) //Fit for y= Ax+B, return (A,B)
{
    double squareX(0), X(0), Y(0), XY(0);
    for (auto i : pointSet)
    {
        X += i.front();
        Y += i.back();
        XY += i.front() * i.back();
        squareX += i.front() * i.front();
    }
    double denominator = squareX - X * X / double(pointSet.size());
    double A = (XY - X * Y / double(pointSet.size())) / denominator;

    // cout << X<< "  " << Y << endl;
    return array<double, 2>({A, Y / double(pointSet.size()) - A * X / double(pointSet.size())});
}

array<double, 2> Fit(TH2D *graphic)
{
    vector<Point2D> pointSet;
    double y_max(0);
    int y_position(0);
    TAxis *axisX = graphic->GetXaxis();
    TAxis *axisY = graphic->GetYaxis();
    // for (int x_temp = 1; x_temp <= graphic->GetNbinsX(); x_temp++)
    for (int x_temp = 1; x_temp <= graphic->GetNbinsX(); x_temp++)

    {
        y_max = 0;
        for (int y_temp = 1; y_temp <= graphic->GetNbinsY(); y_temp++)
        {
            if (graphic->GetBinContent(x_temp, y_temp) > y_max)
            {
                y_position = y_temp;
                y_max = graphic->GetBinContent(x_temp, y_temp);
            }
        }
        if (y_max)
            pointSet.push_back(Point2D({axisX->GetBinCenter(x_temp)+0.15, axisY->GetBinCenter(y_position)}));
        // cout<<axisX->GetBinCenter(x_temp)<< axisY->GetBinCenter(y_position)<<endl;
    }
    return Fit(pointSet);
}

void LinearFit_EnergyMax()
{

    auto graphic = new TFile("Graphic.root", "READ");
    if (!graphic->IsOpen())
    {
        cout << "Can't open Graphic.root" << endl;
        exit(1);
    }

    auto runData = new TFile("RunData.root", "READ");
    if (!runData->IsOpen())
    {
        cout << "Can't open RunData.root" << endl;
        exit(1);
    }
    TTree *primaryData = static_cast<TTree *>(runData->Get("/ntuple/PrimaryData"));
    if (!primaryData)
    {
        cout << "Can't find PrimaryData" << endl;
        exit(1);
    }
    int primaryData_eventNum;
    double primaryData_positionX;
    double primaryData_positionY;
    double primaryData_positionZ;
    double primaryData_directionX;
    double primaryData_directionY;
    double primaryData_directionZ;
    primaryData->SetBranchAddress("EventNum", &primaryData_eventNum);
    primaryData->SetBranchAddress("PrimaryVertexX", &primaryData_positionX);
    primaryData->SetBranchAddress("PrimaryVertexY", &primaryData_positionY);
    primaryData->SetBranchAddress("PrimaryVertexZ", &primaryData_positionZ);
    primaryData->SetBranchAddress("PrimaryDirectionX", &primaryData_directionX);
    primaryData->SetBranchAddress("PrimaryDirectionY", &primaryData_directionY);
    primaryData->SetBranchAddress("PrimaryDirectionZ", &primaryData_directionZ);

    auto fitResult = new TFile("FitResult.root", "RECREATE");
    fitResult->cd();

    array<double, 3> *enterDirection = new array<double, 3>;
    array<double, 3> *enterPoint = new array<double, 3>;

    TTree *fitResultTree = new TTree("FitResult", "FitResult");
    fitResultTree->Branch("EnterPoint", enterPoint);
    fitResultTree->Branch("EnterDirection", enterDirection);

    int numOfEvents = graphic->GetListOfKeys()->GetEntries();
    // cout<<numOfEvents<<endl;

    TH3D *graphic3D = nullptr;
    TH2D *graphic2DXZ = nullptr;
    TH2D *graphic2DYZ = nullptr;
    TH1D *error_position = new TH1D("Position error", "Position error", 100, 0, 10);
    TH1D *error_direction = new TH1D("Direction error", "Direction error", 100, 0, 1);

    Point2D parameterXZ;
    Point2D parameterYZ;

    vector<Position> truePosition;
    vector<Position> trueDirection;
    truePosition.resize(primaryData->GetEntries());
    trueDirection.resize(primaryData->GetEntries());
    for (int i = 0; i < primaryData->GetEntries(); i++)
    {
        primaryData->GetEntry(i);
        truePosition.at(primaryData_eventNum) = Position({primaryData_positionX, primaryData_positionY, primaryData_positionZ});
        trueDirection.at(primaryData_eventNum) = Position({primaryData_directionX, primaryData_directionY, primaryData_directionZ});
    }

    for (int i = 0; i < numOfEvents; i++)
    {
        if (!(i % 10))
            cout << "Number of event: " << i << endl;
        graphic3D = static_cast<TH3D *>(graphic->Get((string("Event_") + std::to_string(i)).c_str()));
        // cout << graphic3D->GetSumOfWeights() << endl;
        if (!graphic3D)
        {
            cout << "Can't find " << string("Event_") + std::to_string(i) << endl;
            exit(1);
        }
        graphic2DXZ = static_cast<TH2D *>(graphic3D->Project3D("xz"));
        graphic2DYZ = static_cast<TH2D *>(graphic3D->Project3D("yz"));
        parameterXZ = Fit(graphic2DXZ);
        parameterYZ = Fit(graphic2DYZ);
        enterPoint->at(0) =parameterXZ.back();
        enterPoint->at(1) =parameterYZ.back();
        enterPoint->at(2) = 0;
        enterDirection->at(0) = parameterXZ.front();
        enterDirection->at(1) = parameterYZ.front();
        enterDirection->at(2) = 1.;
        //归一化
        double sum = Distance(*enterDirection, Position({0, 0, 0}));
        enterDirection->at(0) = enterDirection->at(0) / sum;
        enterDirection->at(1) = enterDirection->at(1) / sum;
        enterDirection->at(2) = enterDirection->at(2) / sum;

        error_position->Fill(Distance(truePosition.at(i), *enterPoint));
        // cout << endl;
        // cout << enterDirection->at(0) << '\t' << enterDirection->at(1) << '\t' << enterDirection->at(2) << endl;
        // cout << trueDirection.at(i).at(0) << '\t' << trueDirection.at(i).at(1) << '\t' << trueDirection.at(i).at(2) << endl;

        error_direction->Fill(Distance(Position({enterDirection->at(0) - trueDirection.at(i).at(0), enterDirection->at(1) - trueDirection.at(i).at(1), enterDirection->at(2) - trueDirection.at(i).at(2)}), Position({0, 0, 0})));
        fitResultTree->Fill();

        delete graphic2DXZ;
        delete graphic2DYZ;
        delete graphic3D;
    }

    TCanvas *canvas = new TCanvas("ErrorPicture", "ErrorPicture", 5);
    error_position->Draw();
    canvas->Print("Error.pdf(");
    error_direction->Draw();
    canvas->Print("Error.pdf)");

    fitResultTree->Write();
    fitResult->Close();
    graphic->Close();
}