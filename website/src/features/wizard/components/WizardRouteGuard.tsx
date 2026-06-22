import { useEffect } from "react";
import { useLocation, useNavigate } from "react-router-dom";
import { useWizard } from "@/features/wizard/context/WizardContext";
import { getFirstBlockedWizardPath } from "@/features/wizard/lib/validation";

export function WizardRouteGuard() {
  const location = useLocation();
  const navigate = useNavigate();
  const { draft } = useWizard();

  useEffect(() => {
    const blockedPath = getFirstBlockedWizardPath(draft, location.pathname);
    if (blockedPath && blockedPath !== location.pathname) {
      navigate(blockedPath, { replace: true });
    }
  }, [draft, location.pathname, navigate]);

  return null;
}
