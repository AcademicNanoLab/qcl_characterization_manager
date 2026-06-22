import { Link, NavLink, Outlet } from "react-router-dom";
import { cn } from "@/lib/utils";
import { WizardRouteGuard } from "@/features/wizard/components/WizardRouteGuard";

const navItems = [
  { href: "/", label: "Home" },
  { href: "/wizard/setup", label: "Setup" },
  { href: "/wizard/measurement-pulsed", label: "Measurement" },
  { href: "/wizard/files/pulsed-liv", label: "Files" },
  { href: "/results", label: "Generate" },
  { href: "/report", label: "Report" },
];

export function AppShell() {
  return (
    <div className="min-h-screen">
      <WizardRouteGuard />
      <header className="border-b border-[var(--color-line)] bg-white/80 backdrop-blur">
        <div className="mx-auto flex max-w-7xl items-center justify-between px-6 py-4">
          <Link to="/" className="text-lg font-semibold">
            Characterization Web Manager
          </Link>
          <nav className="flex flex-wrap gap-2">
            {navItems.map((item) => (
              <NavLink
                key={item.href}
                to={item.href}
                className={({ isActive }) =>
                  cn(
                    "rounded-full px-3 py-2 text-sm transition",
                    isActive ? "bg-[var(--color-ocean)] text-white" : "text-[var(--color-slate)] hover:bg-white",
                  )
                }
              >
                {item.label}
              </NavLink>
            ))}
          </nav>
        </div>
      </header>
      <main className="mx-auto flex max-w-7xl flex-col gap-6 px-6 py-8">
        <Outlet />
      </main>
    </div>
  );
}
