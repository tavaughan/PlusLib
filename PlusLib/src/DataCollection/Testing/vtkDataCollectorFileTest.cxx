/*=Plus=header=begin======================================================
Program: Plus
Copyright (c) Laboratory for Percutaneous Surgery. All rights reserved.
See License.txt for details.
=========================================================Plus=header=end*/ 

/*!
\file This program tests if a recorded tracked ultrasound buffer can be read and replayed from file using vtkDataCollectorFile
*/ 

#include "vtkSmartPointer.h"
#include "vtksys/CommandLineArguments.hxx"
#include "vtkXMLUtilities.h"
#include "vtkMatrix4x4.h"

#include "PlusConfigure.h"
#include "vtkDataCollectorFile.h"
#include "vtkTransformRepository.h"
#include "TrackedFrame.h"

int main( int argc, char** argv )
{

  // Check command line arguments.
  std::string  inputConfigFileName;
  int          verboseLevel = vtkPlusLogger::LOG_LEVEL_DEFAULT;

  vtksys::CommandLineArguments args;
  args.Initialize( argc, argv );

  args.AddArgument( "--input-config-file-name", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &inputConfigFileName, "Name of the input configuration file." );
  args.AddArgument( "--verbose", vtksys::CommandLineArguments::EQUAL_ARGUMENT, &verboseLevel, "Verbose level (1=error only, 2=warning, 3=info, 4=debug 5=trace)" );  

  if ( ! args.Parse() )
  {
    std::cerr << "Problem parsing arguments." << std::endl;
    std::cout << "Help: " << args.GetHelp() << std::endl;
    return 1;
  }

  vtkPlusLogger::Instance()->SetLogLevel( verboseLevel );

  // Prepare and start data collection
  vtkSmartPointer<vtkXMLDataElement> configRootElement = vtkSmartPointer<vtkXMLDataElement>::Take(vtkXMLUtilities::ReadElementFromFile(inputConfigFileName.c_str()));
  if (configRootElement == NULL)
  {	
    LOG_ERROR("Unable to read configuration from file " << inputConfigFileName.c_str()); 
    return 1;
  }

  vtkPlusConfig::GetInstance()->SetDeviceSetConfigurationData(configRootElement);

  vtkSmartPointer<vtkDataCollector> dataCollector = vtkSmartPointer<vtkDataCollector>::New();

  vtkDataCollectorFile* dataCollectorFile = dynamic_cast<vtkDataCollectorFile*>(dataCollector.GetPointer());
  if ( dataCollectorFile == NULL )
  {
    LOG_ERROR("Failed to create the propertype of proper data collector type!");
    return 1;
  }

  dataCollector->ReadConfiguration( configRootElement );

  LOG_DEBUG( "Initializing data collector... " );
  dataCollector->Connect();
  dataCollector->Start();

  if (! dataCollector->GetConnected())
  {
    LOG_ERROR("Unable to start data collection!"); 
    return 1;
  }

  // Create the used objects
  TrackedFrame trackedFrame;

  PlusTransformName referenceToTrackerTransformName("Reference", "Tracker");
  vtkSmartPointer<vtkMatrix4x4> referenceToTrackerTransformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();

  PlusTransformName probeToTrackerTransformName("Probe", "Tracker");
  vtkSmartPointer<vtkMatrix4x4> probeToTrackerTransformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();

  PlusTransformName stylusToTrackerTransformName("Stylus", "Tracker");
  vtkSmartPointer<vtkMatrix4x4> stylusToTrackerTransformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();

  vtkSmartPointer<vtkTransformRepository> transformRepository = vtkSmartPointer<vtkTransformRepository>::New();

  bool valid(false);

  // Check some transforms to ensure that the correct data is returned by the data collector
  // THIS TEST ONLY WORKS WITH THIS SEQUENCE METAFILE: PlusLib\data\TestImages\fCal_Test_Calibration.mha

  // Frame 0001
  dataCollector->GetTrackedFrameByTime(218.8, &trackedFrame);
  transformRepository->SetTransforms(trackedFrame);

  if ( (transformRepository->GetTransform(referenceToTrackerTransformName, referenceToTrackerTransformMatrix, &valid) != PLUS_SUCCESS) || (!valid) )
  {
    std::string transformNameStr;
    referenceToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Unable to get transform " << transformNameStr);
  }
  if ( fabs(referenceToTrackerTransformMatrix->GetElement(0,3) - (-292.088)) > 0.001
    || fabs(referenceToTrackerTransformMatrix->GetElement(1,3) - (60.4261)) > 0.001
    || fabs(referenceToTrackerTransformMatrix->GetElement(2,3) - (-1762.41)) > 0.001 )
  {
    std::string transformNameStr;
    referenceToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Transform " << transformNameStr << " does not match!");
  }

  if ( (transformRepository->GetTransform(probeToTrackerTransformName, probeToTrackerTransformMatrix, &valid) != PLUS_SUCCESS) || (!valid) )
  {
    std::string transformNameStr;
    probeToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Unable to get transform " << transformNameStr);
  }
  if ( fabs(probeToTrackerTransformMatrix->GetElement(0,3) - (-275.514)) > 0.001
    || fabs(probeToTrackerTransformMatrix->GetElement(1,3) - (82.2319)) > 0.001
    || fabs(probeToTrackerTransformMatrix->GetElement(2,3) - (-1701.99)) > 0.001 )
  {
    std::string transformNameStr;
    probeToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Transform " << transformNameStr << " does not match!");
  }

  if ( (transformRepository->GetTransform(stylusToTrackerTransformName, stylusToTrackerTransformMatrix, &valid) != PLUS_SUCCESS) || (valid) )
  {
    std::string transformNameStr;
    stylusToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Unable to get transform " << transformNameStr);
  }

  // Frame 0013
  dataCollector->GetTrackedFrameByTime(222.3, &trackedFrame);
  transformRepository->SetTransforms(trackedFrame);

  if ( (transformRepository->GetTransform(referenceToTrackerTransformName, referenceToTrackerTransformMatrix, &valid) != PLUS_SUCCESS) || (!valid) )
  {
    std::string transformNameStr;
    referenceToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Unable to get transform " << transformNameStr);
  }
  if ( fabs(referenceToTrackerTransformMatrix->GetElement(0,3) - (-292.055)) > 0.001
    || fabs(referenceToTrackerTransformMatrix->GetElement(1,3) - (60.647)) > 0.001
    || fabs(referenceToTrackerTransformMatrix->GetElement(2,3) - (-1762.63)) > 0.001 )
  {
    std::string transformNameStr;
    referenceToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Transform " << transformNameStr << " does not match!");
  }

  if ( (transformRepository->GetTransform(probeToTrackerTransformName, probeToTrackerTransformMatrix, &valid) != PLUS_SUCCESS) || (!valid) )
  {
    std::string transformNameStr;
    probeToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Unable to get transform " << transformNameStr);
  }
  if ( fabs(probeToTrackerTransformMatrix->GetElement(0,3) - (-276.036)) > 0.001
    || fabs(probeToTrackerTransformMatrix->GetElement(1,3) - (82.8124)) > 0.001
    || fabs(probeToTrackerTransformMatrix->GetElement(2,3) - (-1707.32)) > 0.001 )
  {
    std::string transformNameStr;
    probeToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Transform " << transformNameStr << " does not match!");
  }

  if ( (transformRepository->GetTransform(stylusToTrackerTransformName, stylusToTrackerTransformMatrix, &valid) != PLUS_SUCCESS) || (valid) )
  {
    std::string transformNameStr;
    stylusToTrackerTransformName.GetTransformName(transformNameStr);
    LOG_ERROR("Unable to get transform " << transformNameStr);
  }

  dataCollector->Stop();
  dataCollector->Disconnect();

  return 0;
} 