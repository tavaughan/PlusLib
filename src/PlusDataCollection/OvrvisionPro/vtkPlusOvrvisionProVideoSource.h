/*=Plus=header=begin======================================================
  Program: Plus
  Copyright (c) Laboratory for Percutaneous Surgery. All rights reserved.
  See License.txt for details.
=========================================================Plus=header=end*/

#ifndef __vtkPlusOvrvisionProVideoSource_h
#define __vtkPlusOvrvisionProVideoSource_h

#include "vtkPlusDataCollectionExport.h"
#include "vtkPlusDevice.h"

// OvrvisionPro SDK includes
#include <ovrvision_pro.h>

/*!
  \class __vtkPlusOvrvisionProVideoSource_h
  \brief Class for providing video input from the OvrvisionPro stereo camera device
  \ingroup PlusLibDataCollection
*/
class vtkPlusDataCollectionExport vtkPlusOvrvisionProVideoSource : public vtkPlusDevice
{
public:
  static vtkPlusOvrvisionProVideoSource* New();
  vtkTypeMacro(vtkPlusOvrvisionProVideoSource, vtkPlusDevice);
  void PrintSelf(ostream& os, vtkIndent indent);

  virtual bool IsTracker() const
  {
    return false;
  }

  /// Read configuration from xml data
  virtual PlusStatus ReadConfiguration(vtkXMLDataElement* config);
  /// Write configuration to xml data
  virtual PlusStatus WriteConfiguration(vtkXMLDataElement* config);

  /// Perform any completion tasks once configured
  virtual PlusStatus NotifyConfigured();

  std::string GetLeftEyeDataSourceName() const;
  std::string GetRightEyeDataSourceName() const;
  std::string GetProcessingModeName() const;
  std::string GetVendor() const;

  vtkGetVector2Macro(Resolution, int);
  vtkGetMacro(Framerate, int);
  vtkGetMacro(CameraSync, bool);
  vtkSetMacro(Exposure, int);

protected:
  void SetLeftEyeDataSourceName(const std::string& leftEyeDataSourceName);
  void SetRightEyeDataSourceName(const std::string& rightEyeDataSourceName) { this->RightEyeDataSourceName = rightEyeDataSourceName; }
  void SetVendor(const std::string& vendor);
  void SetProcessingModeName(const std::string& processingModeName);

  vtkSetVector2Macro(Resolution, int);
  vtkSetMacro(Framerate, int);
  vtkSetMacro(CameraSync, bool);
  vtkGetMacro(Exposure, int);

  /// Device-specific connect
  virtual PlusStatus InternalConnect();

  /// Device-specific disconnect
  virtual PlusStatus InternalDisconnect();

  /// Device-specific on-update function
  virtual PlusStatus InternalUpdate();

  bool ConfigureRequestedFormat();
  void ConfigureProcessingMode();

protected:
  vtkPlusOvrvisionProVideoSource();
  ~vtkPlusOvrvisionProVideoSource();

protected:
  // Requested capture format
  OVR::OvrvisionPro OvrvisionProHandle;
  OVR::Camprop RequestedFormat;
  OVR::ROI RegionOfInterest;
  OVR::Camqt ProcessingMode;
  bool CameraSync;

  // Cache variables from the config file
  int Resolution[2];
  int Framerate;
  std::string ProcessingModeName;
  bool IsCapturingRGB;
  std::string Vendor;
  int Exposure;

  // Record which data source corresponds to which eye
  std::string LeftEyeDataSourceName;
  std::string RightEyeDataSourceName;

  vtkPlusDataSource* LeftEyeDataSource;
  vtkPlusDataSource* RightEyeDataSource;

private:
  static vtkPlusOvrvisionProVideoSource* ActiveDevice;
  vtkPlusOvrvisionProVideoSource(const vtkPlusOvrvisionProVideoSource&);   // Not implemented.
  void operator=(const vtkPlusOvrvisionProVideoSource&);   // Not implemented.
};

#endif