import type { PropsWithChildren } from "react";
import { WizardSidebar } from "@/features/wizard/components/WizardSidebar";

export function WizardLayout({ children }: PropsWithChildren) {
  return (
    <div className="flex flex-col gap-6 lg:flex-row lg:items-start">
      <WizardSidebar />
      <div className="min-w-0 flex-1">{children}</div>
    </div>
  );
}
