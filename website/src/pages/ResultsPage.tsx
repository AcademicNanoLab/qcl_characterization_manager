import { Card } from "@/components/ui/card";
import { WizardLayout } from "@/components/layout/WizardLayout";
import { LivPlotCard } from "@/features/plot-viewer/components/PlotCard";
import { useProcessedResults } from "@/features/plot-viewer/lib/useProcessedResults";

export function ResultsPage() {
  const { livResults } = useProcessedResults();
  const pulsedLivResults = livResults.filter((result) => result.datasetName === "Pulsed LIV");

  return (
    <WizardLayout>
      <Card className="mb-2 bg-[linear-gradient(135deg,#eff6ff_0%,#ffffff_55%,#f0fdfa_100%)]">
        <h2 className="text-2xl font-semibold">Generate Image File</h2>
      </Card>
      {pulsedLivResults.length === 0 ? (
        <Card>
          <h2 className="text-xl font-semibold">No processed results yet</h2>
          <p className="mt-2 text-sm text-[var(--color-slate)]">
            Add the required trace files in the wizard steps before generating the image file.
          </p>
        </Card>
      ) : null}
      {pulsedLivResults.map((result) => <LivPlotCard key={result.datasetName} result={result} />)}
    </WizardLayout>
  );
}
