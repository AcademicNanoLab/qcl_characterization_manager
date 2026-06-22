import { measurementSchema, setupSchema } from "@/features/measurement-setup/forms/schemas";
import type { WizardDraft } from "@/types/contracts";

export function isSetupStepValid(draft: WizardDraft) {
  return setupSchema.safeParse({
    author: draft.projectMetadata.author,
    date: draft.projectMetadata.date,
    waferNumber: draft.projectMetadata.device.waferNumber,
    deviceName: draft.projectMetadata.device.deviceName,
    design: draft.projectMetadata.device.design,
    waveguide: draft.projectMetadata.device.waveguide,
    approxEmissionFrequencyThz: draft.projectMetadata.device.approxEmissionFrequencyThz,
    width: draft.projectMetadata.device.dimensions.width,
    length: draft.projectMetadata.device.dimensions.length,
    height: draft.projectMetadata.device.dimensions.height,
  }).success;
}

export function isMeasurementModeValid(draft: WizardDraft, mode: "pulsed" | "cw") {
  return measurementSchema.safeParse({
    pulsedCryostat: draft.measurementSetup.pulsed.cryostat,
    pulsedLivDetector: draft.measurementSetup.pulsed.livDetector,
    pulsedSpectraDetector: draft.measurementSetup.pulsed.spectraDetector,
    pulsedPowerSupply: draft.measurementSetup.pulsed.powerSupply,
    pulsedSpectrometer: draft.measurementSetup.pulsed.spectrometer,
    pulsedPowerScale: draft.measurementSetup.pulsed.powerScale,
    pulsedDriveFrequency: draft.measurementSetup.pulsed.driveFrequency,
    pulsedDutyCycle: draft.measurementSetup.pulsed.dutyCycle,
    pulsedGateFrequency: draft.measurementSetup.pulsed.gateFrequency,
    pulsedTMax: draft.measurementSetup.pulsed.tMax,
    pulsedFMin: draft.measurementSetup.pulsed.fMin,
    pulsedFMax: draft.measurementSetup.pulsed.fMax,
    pulsedTFix: draft.measurementSetup.pulsed.tFix,
    cwCryostat: draft.measurementSetup.cw.cryostat,
    cwLivDetector: draft.measurementSetup.cw.livDetector,
    cwSpectraDetector: draft.measurementSetup.cw.spectraDetector,
    cwPowerSupply: draft.measurementSetup.cw.powerSupply,
    cwSpectrometer: draft.measurementSetup.cw.spectrometer,
    cwPowerScale: draft.measurementSetup.cw.powerScale,
    cwTMax: draft.measurementSetup.cw.tMax,
    cwFMin: draft.measurementSetup.cw.fMin,
    cwFMax: draft.measurementSetup.cw.fMax,
    cwTFix: draft.measurementSetup.cw.tFix,
  }).success && (mode === "pulsed" ? true : draft.measurementSetup.cw.powerScale > 0);
}

export function getFirstBlockedWizardPath(draft: WizardDraft, targetPath: string) {
  const setupValid = isSetupStepValid(draft);
  const pulsedValid = isMeasurementModeValid(draft, "pulsed");
  const cwValid = isMeasurementModeValid(draft, "cw");

  if (targetPath === "/" || targetPath === "/wizard/setup") {
    return null;
  }

  if (!setupValid) {
    return "/wizard/setup";
  }

  if (targetPath === "/wizard/measurement-pulsed") {
    return null;
  }

  const pulsedAndForwardPaths = [
    "/wizard/files/pulsed-liv",
    "/wizard/files/pulsed-ftir-fixed-temperature",
    "/wizard/files/pulsed-ftir-fixed-current",
    "/wizard/measurement-cw",
    "/wizard/files/cw-liv",
    "/wizard/files/cw-ftir-fixed-temperature",
    "/wizard/files/cw-ftir-fixed-current",
    "/results",
    "/report",
  ];

  if (pulsedAndForwardPaths.some((path) => targetPath.startsWith(path)) && !pulsedValid) {
    return "/wizard/measurement-pulsed";
  }

  const cwAndForwardPaths = [
    "/wizard/files/cw-liv",
    "/wizard/files/cw-ftir-fixed-temperature",
    "/wizard/files/cw-ftir-fixed-current",
    "/results",
    "/report",
  ];

  if (cwAndForwardPaths.some((path) => targetPath.startsWith(path)) && !cwValid) {
    return "/wizard/measurement-cw";
  }

  return null;
}
