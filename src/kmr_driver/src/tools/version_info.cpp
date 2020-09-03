/**                                                                           !
  * @file /kmr_driver/src/tools/version_info.cpp
  *
  * @brief Tools/utility program to retriving version info. of kmr.
  *
 **/

/*****************************************************************************
 * Includes
 ****************************************************************************/

#include <string>
#include <ecl/time.hpp>
#include <ecl/sigslots.hpp>
#include <ecl/command_line.hpp>
#include "kmr_driver/kmr.hpp"

/*****************************************************************************
** Classes
*****************************************************************************/

class KmrManager {
public:
  KmrManager(const std::string &device_port) :
    acquired(false),
    slot_version_info(&KmrManager::processVersionInfo, *this)
  {
    kmr::Parameters parameters;
    parameters.sigslots_namespace = "/kmr"; // configure the first part of the sigslot namespace
    parameters.device_port = device_port;    // the serial port to connect to (windows COM1..)
    kmr.init(parameters);
    kmr.enable();
    slot_version_info.connect("/kmr/version_info");
  }

  ~KmrManager() {
    kmr.disable();
  }

  void processVersionInfo(const kmr::VersionInfo &version_info) {
    hardware = kmr::VersionInfo::toString(version_info.hardware);
    firmware = kmr::VersionInfo::toString(version_info.firmware);
    //software = kmr::VersionInfo::toString(version_info.software);
    software = kmr::VersionInfo::getSoftwareVersion();
    udid = kmr::VersionInfo::toString(version_info.udid0, version_info.udid1, version_info.udid2);
    acquired = true;
  }

  bool isAcquired() { return acquired; }
  std::string& getHardwareVersion() { return hardware; }
  std::string& getFirmwareVersion() { return firmware; }
  std::string& getSoftwareVersion() { return software; }
  std::string& getUDID() { return udid; }

private:
  volatile bool acquired;
  kmr::Kmr kmr;
  std::string hardware, firmware, software, udid;
  ecl::Slot<const kmr::VersionInfo&> slot_version_info;
};

/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char** argv)
{
  ecl::CmdLine cmd_line("version_info program", ' ', "0.2");
  ecl::UnlabeledValueArg<std::string> device_port("device_port", "Path to device file of serial port to open, connected to the kmr", false, "/dev/kmr", "string");
  cmd_line.add(device_port);
  cmd_line.parse(argc, argv);
  //std::cout << "device_port: " << device_port.getValue() << std::endl;

  std::cout << "Version Info:" << std::endl;
  KmrManager kmr_manager(device_port.getValue());

  while (!kmr_manager.isAcquired());
  std::cout << " * Hardware Version: " << kmr_manager.getHardwareVersion() << std::endl;
  std::cout << " * Firmware Version: " << kmr_manager.getFirmwareVersion() << std::endl;
  std::cout << " * Software Version: " << kmr_manager.getSoftwareVersion() << std::endl;
  std::cout << " * Unique Device ID: " << kmr_manager.getUDID() << std::endl;

  return 0;
}
