import { useMemo } from "react";
import { parseLivFile } from "@/core/parsers/livParser";
import { parseSpectraFile } from "@/core/parsers/spectraParser";
import { processLivDataset } from "@/core/processing/livProcessing";
import { processSpectraDataset } from "@/core/processing/spectraProcessing";
import { useWizard } from "@/features/wizard/context/WizardContext";
import type { LivDataset, ProcessedLivResult, ProcessedSpectraResult, SpectraDataset } from "@/types/contracts";

export function useProcessedResults() {
  const { draft } = useWizard();

  return useMemo(() => {
    const livResults: ProcessedLivResult[] = [];
    const spectraResults: ProcessedSpectraResult[] = [];

    const pulsedLivFiles = draft.traceAssignments.pulsedLiv.files;
    if (pulsedLivFiles.length > 0) {
      const dataset: LivDataset = {
        kind: "liv",
        datasetName: draft.traceAssignments.pulsedLiv.datasetName,
        traceVariableLabel: draft.traceAssignments.pulsedLiv.traceVariableLabel,
        traces: pulsedLivFiles.map(parseLivFile),
      };
      livResults.push(processLivDataset(dataset, draft.measurementSetup.pulsed.powerScale, draft.projectMetadata.device.dimensions));
    }

    const cwLivFiles = draft.traceAssignments.cwLiv.files;
    if (cwLivFiles.length > 0) {
      const dataset: LivDataset = {
        kind: "liv",
        datasetName: draft.traceAssignments.cwLiv.datasetName,
        traceVariableLabel: draft.traceAssignments.cwLiv.traceVariableLabel,
        traces: cwLivFiles.map(parseLivFile),
      };
      livResults.push(processLivDataset(dataset, draft.measurementSetup.cw.powerScale, draft.projectMetadata.device.dimensions));
    }

    for (const key of [
      "pulsedSpectraFixedTemperature",
      "pulsedSpectraFixedCurrent",
      "cwSpectraFixedTemperature",
      "cwSpectraFixedCurrent",
    ] as const) {
      const assignment = draft.traceAssignments[key];
      if (assignment.files.length === 0) continue;
      const dataset: SpectraDataset = {
        kind: "spectra",
        datasetName: assignment.datasetName,
        traceVariableLabel: assignment.traceVariableLabel,
        traces: assignment.files.map(parseSpectraFile),
      };
      spectraResults.push(processSpectraDataset(dataset));
    }

    return { livResults, spectraResults };
  }, [draft]);
}
