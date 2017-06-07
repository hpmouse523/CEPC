#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"
#include "TAxis.h"

using std::cout;
using std::string;
using std::endl;
using std::vector;

void DataToPicture()
{
    auto file = new TFile("RunData.root", "READ");

    TH3D *pictureExample = static_cast<TH3D *>(file->Get("/histo/Detection example"));
    if (!pictureExample)
    {
        cout << "Can't open /histo/Detection example" << endl;
        exit(1);
    }

    TTree *primaryData = static_cast<TTree *>(file->Get("/ntuple/PrimaryData"));
    if (!primaryData)
    {
        cout << "Can't open /ntuple/PrimaryData" << endl;
        exit(1);
    }

    TTree *stepData = static_cast<TTree *>(file->Get("/ntuple/StepData"));
    if (!stepData)
    {
        cout << "Can't open /ntuple/StepData" << endl;
        exit(1);
    }
    int primaryEventNum;
    double primaryVertexX, primaryVertexY, primaryVertexZ;
    double primaryDirectionX, primaryDirectionY, primaryDirectionZ;
    double primaryEnergy;

    int eventNum;
    double positionX, positionY, positionZ;
    double energy;

    stepData->SetBranchAddress("EventNum", &eventNum);
    stepData->SetBranchAddress("PositionX", &positionX);
    stepData->SetBranchAddress("PositionY", &positionY);
    stepData->SetBranchAddress("PositionZ", &positionZ);
    stepData->SetBranchAddress("Energy", &energy);

    int numOfEvents = primaryData->GetEntries();

    vector<TH3D *> picture;
    int vectorSize = 50;
    bool needLoop = false;
    if (numOfEvents > vectorSize)
        {
            needLoop = true;
            vectorSize=numOfEvents/5;
        }

    if (needLoop)
        picture.resize(vectorSize);
    else
        picture.resize(numOfEvents);

    const string nameOfEvent("Event_");
    TAxis *axisX = pictureExample->GetXaxis();
    TAxis *axisY = pictureExample->GetYaxis();
    TAxis *axisZ = pictureExample->GetZaxis();

    TFile *new_file = new TFile("Graphic.root", "RECREATE");
    new_file->cd();

    int count = 0;
    if (needLoop)
    {
        for (count = 0; count < numOfEvents - vectorSize; count += vectorSize)
        {
            cout << "Number of Event: " << count << endl;
            for (int i = 0; i < vectorSize; i++)
                picture.at(i) = new TH3D((nameOfEvent + std::to_string(count + i)).c_str(),
                                         (nameOfEvent + std::to_string(count + i)).c_str(),
                                         axisX->GetNbins(), axisX->GetXmin(), axisX->GetXmax(),
                                         axisY->GetNbins(), axisY->GetXmin(), axisY->GetXmax(),
                                         axisZ->GetNbins(), axisZ->GetXmin(), axisZ->GetXmax());
            for (int j = 0; j < stepData->GetEntries(); j++)
            {
                stepData->GetEntry(j);
                if (eventNum >= count && eventNum < count + vectorSize)
                {
                    picture.at(eventNum - count)->Fill(positionX, positionY, positionZ, energy);
                }
            }
            for (int i = 0; i < vectorSize; i++)
            {
                picture.at(i)->Write();
                delete picture.at(i);
            }
        }

        for (int i = 0; i < numOfEvents - count; i++)
            picture.at(i) = new TH3D((nameOfEvent + std::to_string(count + i)).c_str(),
                                     (nameOfEvent + std::to_string(count + i)).c_str(),
                                     axisX->GetNbins(), axisX->GetXmin(), axisX->GetXmax(),
                                     axisY->GetNbins(), axisY->GetXmin(), axisY->GetXmax(),
                                     axisZ->GetNbins(), axisZ->GetXmin(), axisZ->GetXmax());
        for (int j = 0; j < stepData->GetEntries(); j++)
        {
            stepData->GetEntry(j);
            if (eventNum >= count && eventNum < numOfEvents)
            {
                picture.at(eventNum - count)->Fill(positionX, positionY, positionZ, energy);
            }
        }
        for (int i = 0; i < numOfEvents - count; i++)
        {
            picture.at(i)->Write();
            delete picture.at(i);
        }
    }

    else
    {
        for (int i = 0; i < numOfEvents; i++)
            picture.at(i) = new TH3D((nameOfEvent + std::to_string(i)).c_str(),
                                     (nameOfEvent + std::to_string(i)).c_str(),
                                     axisX->GetNbins(), axisX->GetXmin(), axisX->GetXmax(),
                                     axisY->GetNbins(), axisY->GetXmin(), axisY->GetXmax(),
                                     axisZ->GetNbins(), axisZ->GetXmin(), axisZ->GetXmax());
        for (int j = 0; j < stepData->GetEntries(); j++)
        {
            stepData->GetEntry(j);
            picture.at(eventNum)->Fill(positionX, positionY, positionZ, energy);
        }
        for (int i = 0; i < numOfEvents; i++)
        {
            picture.at(i)->Write();
            delete picture.at(i);
        }
    }
    new_file->Close();
    file->Close();
}