##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = blueline
  PLATFORM_GUID                  = 3B0C9F4E-6E1D-4C8A-9F52-B1C1D8450003
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/bluelinePkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = bluelinePkg/blueline.fdf

  #
  # 1: the stock XBL DisplayDxe initializes the panel (as polaris). 0: SimpleFbDxe takes over the framebuffer
  # ABL left at 0x9D400000. The build command line (-D USE_CUSTOM_DISPLAY_DRIVER=...) selects one.
  #
!ifndef USE_CUSTOM_DISPLAY_DRIVER
  DEFINE USE_CUSTOM_DISPLAY_DRIVER = 1
!endif

  #
  # 1: load the stock Pixel 3 XBL drivers (SMEM, clocks, RPMh, PMIC, UFS, USB, buttons, ...), as polaris.
  # 0: Mu drivers only (as sheng's SHENG_VENDOR_HW = 0): no UFS and no keys, but nothing touches the panel ABL
  #    left scanning out, and DRAM comes from ABL's FDT (BluelineBootMemoryDxe).
  #
!ifndef BLUELINE_VENDOR_HW
  DEFINE BLUELINE_VENDOR_HW = 1
!endif

!include NapaliPkg/NapaliPkg.dsc.inc

!if ($(USE_CUSTOM_DISPLAY_DRIVER) == 1) && ($(BLUELINE_VENDOR_HW) == 0)
  !error The stock DisplayDxe chain needs BLUELINE_VENDOR_HW = 1
!endif

[PcdsFixedAtBuild]
  #
  # DDR Memory
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0x9FF90000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Google"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Pixel 3"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"blueline"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Pixel_3_blueline"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"B1 DVT1.1"

  #
  # Simple Frame Buffer
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2160
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
!if $(BLUELINE_VENDOR_HW) == 0
  # As sheng: without the stock drivers nothing reads the HOBs of the stock UEFI's leftover FD window.
  gQcomPkgTokenSpaceGuid.PcdBuildXblHobs|FALSE
!endif

[Components]
  #
  # UniXNU boot manager and XNU loader (as uefi-sheng)
  #
  UniXNUPkg/Application/UniXNU/UniXNU.inf
  UniXNUPkg/Driver/UniXNUDxe/UniXNUDxe.inf
  UniXNUPkg/Driver/UniXNURamDiskDxe/UniXNURamDiskDxe.inf
  DarwinBootPkg/Driver/DarwinBoot.inf
  bluelinePkg/Drivers/BluelineBootMemoryDxe/BluelineBootMemoryDxe.inf

[LibraryClasses]
  UniXNULib|UniXNUPkg/Library/UniXNULib/UniXNULib.inf
  FdtLib|MdePkg/Library/BaseFdtLib/BaseFdtLib.inf

  #
  # Memory Libraries
  #
  MemoryMapLib|bluelinePkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|bluelinePkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
