import type { MeasurementKind } from "@/types/contracts";

export type WizardStep = {
  label: string;
  href: string;
  match?: string[];
  kind?: MeasurementKind;
};

export const wizardSteps: WizardStep[] = [
  { label: "Setup Parameters", href: "/wizard/setup", match: ["/wizard/setup"] },
  {
    label: "Measurement Setup (pulsed)",
    href: "/wizard/measurement-pulsed",
    match: ["/wizard/measurement-pulsed"],
  },
  {
    label: "Pulsed LIV",
    href: "/wizard/files/pulsed-liv",
    match: ["/wizard/files/pulsed-liv"],
    kind: "pulsedLiv",
  },
  {
    label: "Pulsed FTIR - fixed temperature",
    href: "/wizard/files/pulsed-ftir-fixed-temperature",
    match: ["/wizard/files/pulsed-ftir-fixed-temperature"],
    kind: "pulsedSpectraFixedTemperature",
  },
  {
    label: "Pulsed FTIR - fixed current",
    href: "/wizard/files/pulsed-ftir-fixed-current",
    match: ["/wizard/files/pulsed-ftir-fixed-current"],
    kind: "pulsedSpectraFixedCurrent",
  },
  {
    label: "Measurement Setup (CW)",
    href: "/wizard/measurement-cw",
    match: ["/wizard/measurement-cw"],
  },
  {
    label: "CW LIV",
    href: "/wizard/files/cw-liv",
    match: ["/wizard/files/cw-liv"],
    kind: "cwLiv",
  },
  {
    label: "CW FTIR - fixed temperature",
    href: "/wizard/files/cw-ftir-fixed-temperature",
    match: ["/wizard/files/cw-ftir-fixed-temperature"],
    kind: "cwSpectraFixedTemperature",
  },
  {
    label: "CW FTIR - fixed current",
    href: "/wizard/files/cw-ftir-fixed-current",
    match: ["/wizard/files/cw-ftir-fixed-current"],
    kind: "cwSpectraFixedCurrent",
  },
  { label: "Generate Image File", href: "/results", match: ["/results"] },
];

export const fileStepOrder: MeasurementKind[] = [
  "pulsedLiv",
  "pulsedSpectraFixedTemperature",
  "pulsedSpectraFixedCurrent",
  "cwLiv",
  "cwSpectraFixedTemperature",
  "cwSpectraFixedCurrent",
];

export function getFileStepHref(kind: MeasurementKind) {
  const step = wizardSteps.find((item) => item.kind === kind);
  return step?.href ?? "/wizard/files/pulsed-liv";
}

export function getWizardStepIndex(pathname: string) {
  return wizardSteps.findIndex((step) => step.match?.some((prefix) => pathname.startsWith(prefix)));
}
