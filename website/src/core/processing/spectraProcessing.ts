import type { ProcessedSpectraResult, SpectraDataset } from "@/types/contracts";

export function processSpectraDataset(dataset: SpectraDataset): ProcessedSpectraResult {
  const traces = [...dataset.traces].sort((left, right) => left.traceValue - right.traceValue);
  const frequencies = traces.flatMap((trace) => trace.points.map((point) => point.frequencyThz));

  return {
    datasetName: dataset.datasetName,
    traceVariableLabel: dataset.traceVariableLabel,
    traces: traces.map((trace) => {
      const rawPeak = Math.max(...trace.points.map((point) => point.intensity));
      const peak = rawPeak > 0 ? rawPeak : 1;
      const peakPoint = trace.points.reduce((best, current) =>
        current.intensity > best.intensity ? current : best,
      );

      return {
        ...trace,
        normalizedIntensities: trace.points.map((point) => point.intensity / peak),
        peakFrequencyThz: peakPoint.frequencyThz,
      };
    }),
    frequencyRange: {
      min: Math.min(...frequencies),
      max: Math.max(...frequencies),
    },
  };
}
