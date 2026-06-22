import { WizardProvider } from "@/features/wizard/context/WizardContext";
import { AppRouter } from "./router";

export default function App() {
  return (
    <WizardProvider>
      <AppRouter />
    </WizardProvider>
  );
}
