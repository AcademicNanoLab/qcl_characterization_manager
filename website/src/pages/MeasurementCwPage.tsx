import { WizardLayout } from "@/components/layout/WizardLayout";
import { MeasurementSetupForm } from "@/features/measurement-setup/forms/MeasurementSetupForm";

export function MeasurementCwPage() {
  return (
    <WizardLayout>
      <MeasurementSetupForm mode="cw" />
    </WizardLayout>
  );
}
