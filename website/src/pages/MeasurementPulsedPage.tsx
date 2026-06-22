import { WizardLayout } from "@/components/layout/WizardLayout";
import { MeasurementSetupForm } from "@/features/measurement-setup/forms/MeasurementSetupForm";

export function MeasurementPulsedPage() {
  return (
    <WizardLayout>
      <MeasurementSetupForm mode="pulsed" />
    </WizardLayout>
  );
}
