import { useNavigate } from "react-router-dom";
import { Button } from "@/components/ui/button";
import { useWizard } from "@/features/wizard/context/WizardContext";
import { fileStepOrder, getFileStepHref } from "@/features/wizard/lib/wizardSteps";
import type { MeasurementKind } from "@/types/contracts";
import { FileAssignmentSection } from "./FileAssignmentSection";

interface FileImportWorkspaceProps {
  kind: MeasurementKind;
}

export function FileImportWorkspace({ kind }: FileImportWorkspaceProps) {
  const navigate = useNavigate();
  const { draft, setAssignmentFiles, setAssignmentNotes } = useWizard();
  const assignment = draft.traceAssignments[kind];
  const currentIndex = fileStepOrder.indexOf(kind);
  const previousKind = currentIndex > 0 ? fileStepOrder[currentIndex - 1] : undefined;
  const nextKind = currentIndex < fileStepOrder.length - 1 ? fileStepOrder[currentIndex + 1] : undefined;

  return (
    <div className="grid gap-6">
      <FileAssignmentSection
        kind={kind}
        assignment={assignment}
        onChange={(files) => setAssignmentFiles(kind, files)}
        onNotesChange={(notes) => setAssignmentNotes(kind, notes)}
      />
      <div className="flex flex-wrap justify-between gap-3">
        <div>
          {previousKind ? (
            <Button type="button" variant="outline" onClick={() => navigate(getFileStepHref(previousKind))}>
              Previous
            </Button>
          ) : null}
        </div>
        <div className="flex gap-3">
          {nextKind ? (
            <Button type="button" onClick={() => navigate(getFileStepHref(nextKind))}>
              Next
            </Button>
          ) : (
            <Button type="button" onClick={() => navigate("/results")}>
              Generate Image File
            </Button>
          )}
        </div>
      </div>
    </div>
  );
}
