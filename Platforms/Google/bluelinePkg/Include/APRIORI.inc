APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

!if $(BLUELINE_VENDOR_HW) == 1
  INF Binaries/blueline/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

!endif
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

!if $(BLUELINE_VENDOR_HW) == 1
  INF Binaries/blueline/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/DALSys/DALSys.inf
  INF Binaries/blueline/QcomPkg/Drivers/HWIODxeDriver/HWIODxeDriver.inf
  INF Binaries/blueline/QcomPkg/Drivers/ChipInfo/ChipInfo.inf
  INF Binaries/blueline/QcomPkg/Drivers/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  INF Binaries/blueline/QcomPkg/Drivers/HALIOMMU/HALIOMMU.inf

!endif
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

!if $(BLUELINE_VENDOR_HW) == 1
  INF Binaries/blueline/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
!endif
!if ($(BLUELINE_VENDOR_HW) == 1) && ($(USE_CUSTOM_DISPLAY_DRIVER) == 1)
  INF Binaries/blueline/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

!if $(BLUELINE_VENDOR_HW) == 1
  INF Binaries/blueline/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/TzDxe/TzDxe.inf

!endif
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

!if $(BLUELINE_VENDOR_HW) == 1
  INF Binaries/blueline/QcomPkg/Drivers/DALTLMM/DALTLMM.inf
  INF Binaries/blueline/QcomPkg/Drivers/SPMI/SPMI.inf
  INF Binaries/blueline/QcomPkg/Drivers/PmicDxe/PmicDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/QcomChargerDxeLA/QcomChargerDxeLA.inf
  INF Binaries/blueline/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

!endif
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

!if $(BLUELINE_VENDOR_HW) == 1
  INF Binaries/blueline/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf

!endif
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(BLUELINE_VENDOR_HW) == 1
  INF Binaries/blueline/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!endif
!if ($(BLUELINE_VENDOR_HW) == 1) && ($(USE_CUSTOM_DISPLAY_DRIVER) == 1)
  INF Binaries/blueline/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
  INF QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

!if $(BLUELINE_VENDOR_HW) == 1
  INF Binaries/blueline/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/RngDxe/RngDxe.inf
  INF Binaries/blueline/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
!endif
}
