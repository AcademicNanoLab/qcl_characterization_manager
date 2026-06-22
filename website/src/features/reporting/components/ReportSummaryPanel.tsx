import { Card } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { exportDraftAsJson } from "@/core/export/projectExport";
import { useWizard } from "@/features/wizard/context/WizardContext";
import { useProcessedResults } from "@/features/plot-viewer/lib/useProcessedResults";

export function ReportSummaryPanel() {
  const { draft } = useWizard();
  const { livResults, spectraResults } = useProcessedResults();

  return (
    <div className="grid gap-6 lg:grid-cols-[1.2fr_0.8fr]">
      <Card>
        <h2 className="text-xl font-semibold">Report-ready summary</h2>
        <div className="mt-4 grid gap-3 text-sm text-[var(--color-slate)]">
          <p><strong className="text-[var(--color-ink)]">Author:</strong> {draft.projectMetadata.author || "Unspecified"}</p>
          <p><strong className="text-[var(--color-ink)]">Wafer:</strong> {draft.projectMetadata.device.waferNumber || "Unspecified"}</p>
          <p><strong className="text-[var(--color-ink)]">Device:</strong> {draft.projectMetadata.device.deviceName || "Unspecified"}</p>
          <p><strong className="text-[var(--color-ink)]">Design:</strong> {draft.projectMetadata.device.design}</p>
          <p><strong className="text-[var(--color-ink)]">Plots prepared:</strong> {livResults.length + spectraResults.length}</p>
        </div>
      </Card>
      <Card className="bg-[var(--color-panel)]">
        <h3 className="text-lg font-semibold">Prototype export</h3>
        <p className="mt-3 text-sm leading-6 text-[var(--color-slate)]">
          This first version preserves the workflow and processing structure. Instead of PDF generation, it currently
          exports the browser project state as structured JSON for verification and future backend/report integration.
        </p>
        <Button className="mt-5" onClick={() => exportDraftAsJson(draft)}>
          Export Prototype JSON
        </Button>
      </Card>
    </div>
  );
}
