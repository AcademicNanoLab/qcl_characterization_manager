/* eslint-disable react-refresh/only-export-components */
import { createContext, useContext, useEffect, useMemo, useState, type PropsWithChildren } from "react";
import { createInitialDraft } from "@/core/domain/defaults";
import {
  clearPersistedDraft,
  downloadDraftJson,
  loadPersistedDraft,
  normalizeDraft,
  savePersistedDraft,
} from "@/lib/wizardDraftPersistence";
import type {
  MeasurementKind,
  MeasurementSetup,
  ProjectMetadata,
  UploadedMeasurementFile,
  WizardDraft,
} from "@/types/contracts";

interface WizardContextValue {
  draft: WizardDraft;
  setProjectMetadata: (metadata: ProjectMetadata) => void;
  setMeasurementSetup: (setup: MeasurementSetup) => void;
  setAssignmentFiles: (kind: MeasurementKind, files: UploadedMeasurementFile[]) => void;
  setAssignmentNotes: (kind: MeasurementKind, notes: string) => void;
  replaceDraft: (nextDraft: WizardDraft) => void;
  exportDraft: () => void;
  clearSavedDraft: () => void;
  resetDraft: () => void;
}

const WizardContext = createContext<WizardContextValue | null>(null);

export function WizardProvider({ children }: PropsWithChildren) {
  const [draft, setDraft] = useState<WizardDraft>(loadPersistedDraft);

  useEffect(() => {
    savePersistedDraft(draft);
  }, [draft]);

  const value = useMemo<WizardContextValue>(
    () => ({
      draft,
      setProjectMetadata: (projectMetadata) => setDraft((current) => ({ ...current, projectMetadata })),
      setMeasurementSetup: (measurementSetup) => setDraft((current) => ({ ...current, measurementSetup })),
      setAssignmentFiles: (kind, files) =>
        setDraft((current) => ({
          ...current,
          traceAssignments: {
            ...current.traceAssignments,
            [kind]: {
              ...current.traceAssignments[kind],
              files,
            },
          },
        })),
      setAssignmentNotes: (kind, notes) =>
        setDraft((current) => ({
          ...current,
          traceAssignments: {
            ...current.traceAssignments,
            [kind]: {
              ...current.traceAssignments[kind],
              notes,
            },
          },
        })),
      replaceDraft: (nextDraft) => setDraft(normalizeDraft(nextDraft)),
      exportDraft: () => downloadDraftJson(draft),
      clearSavedDraft: () => clearPersistedDraft(),
      resetDraft: () => setDraft(createInitialDraft()),
    }),
    [draft],
  );

  return <WizardContext.Provider value={value}>{children}</WizardContext.Provider>;
}

export function useWizard() {
  const context = useContext(WizardContext);
  if (!context) {
    throw new Error("useWizard must be used within WizardProvider");
  }

  return context;
}
