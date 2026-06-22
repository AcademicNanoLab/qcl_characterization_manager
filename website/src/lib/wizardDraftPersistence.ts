import { createInitialDraft } from "@/core/domain/defaults";
import type { MeasurementKind, TraceAssignment, WizardDraft } from "@/types/contracts";

const STORAGE_KEY = "qcl-characterization-web-draft";

function mergeTraceAssignments(
  baseAssignments: Record<MeasurementKind, TraceAssignment>,
  savedAssignments?: Partial<Record<MeasurementKind, Partial<TraceAssignment>>>,
) {
  const merged = { ...baseAssignments };

  if (!savedAssignments) {
    return merged;
  }

  (Object.keys(baseAssignments) as MeasurementKind[]).forEach((kind) => {
    const saved = savedAssignments[kind];
    if (!saved) {
      return;
    }

    merged[kind] = {
      ...baseAssignments[kind],
      ...saved,
      files: Array.isArray(saved.files) ? saved.files : baseAssignments[kind].files,
      notes: typeof saved.notes === "string" ? saved.notes : baseAssignments[kind].notes,
    };
  });

  return merged;
}

export function normalizeDraft(candidate: unknown): WizardDraft {
  const base = createInitialDraft();

  if (!candidate || typeof candidate !== "object") {
    return base;
  }

  const saved = candidate as Partial<WizardDraft>;

  return {
    projectMetadata: {
      ...base.projectMetadata,
      ...saved.projectMetadata,
      device: {
        ...base.projectMetadata.device,
        ...saved.projectMetadata?.device,
        dimensions: {
          ...base.projectMetadata.device.dimensions,
          ...saved.projectMetadata?.device?.dimensions,
        },
      },
    },
    measurementSetup: {
      pulsed: {
        ...base.measurementSetup.pulsed,
        ...saved.measurementSetup?.pulsed,
      },
      cw: {
        ...base.measurementSetup.cw,
        ...saved.measurementSetup?.cw,
      },
    },
    traceAssignments: mergeTraceAssignments(base.traceAssignments, saved.traceAssignments),
  };
}

export function loadPersistedDraft() {
  if (typeof window === "undefined") {
    return createInitialDraft();
  }

  const raw = window.localStorage.getItem(STORAGE_KEY);
  if (!raw) {
    return createInitialDraft();
  }

  try {
    return normalizeDraft(JSON.parse(raw));
  } catch {
    return createInitialDraft();
  }
}

export function savePersistedDraft(draft: WizardDraft) {
  if (typeof window === "undefined") {
    return;
  }

  window.localStorage.setItem(STORAGE_KEY, JSON.stringify(draft, null, 2));
}

export function clearPersistedDraft() {
  if (typeof window === "undefined") {
    return;
  }

  window.localStorage.removeItem(STORAGE_KEY);
}

export function downloadDraftJson(draft: WizardDraft) {
  if (typeof window === "undefined") {
    return;
  }

  const blob = new Blob([JSON.stringify(draft, null, 2)], { type: "application/json" });
  const url = URL.createObjectURL(blob);
  const anchor = document.createElement("a");
  anchor.href = url;
  anchor.download = "qcl-characterization-session.json";
  anchor.click();
  URL.revokeObjectURL(url);
}
