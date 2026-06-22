import { zodResolver } from "@hookform/resolvers/zod";
import { useForm, type UseFormRegisterReturn } from "react-hook-form";
import { useNavigate } from "react-router-dom";
import type { z } from "zod";
import {
  cryostatOptions,
  cwPowerSupplyOptions,
  detectorOptions,
  pulsedPowerSupplyOptions,
  spectraDetectorOptions,
  spectrometerOptions,
} from "@/assets/equipmentOptions";
import { FormField } from "@/components/forms/FormField";
import { Button } from "@/components/ui/button";
import { Card } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { Select } from "@/components/ui/select";
import { useWizard } from "@/features/wizard/context/WizardContext";
import { measurementSchema } from "./schemas";

type MeasurementValues = z.infer<typeof measurementSchema>;

function OptionSelect({
  options,
  register,
}: {
  options: string[];
  register: UseFormRegisterReturn;
}) {
  return (
    <Select {...register}>
      {options.map((option) => (
        <option key={option} value={option}>
          {option}
        </option>
      ))}
    </Select>
  );
}

function FieldGrid({
  prefix,
  register,
}: {
  prefix: "pulsed" | "cw";
  register: ReturnType<typeof useForm<MeasurementValues>>["register"];
}) {
  const title = prefix === "pulsed" ? "Measurement Setup (pulsed)" : "Measurement Setup (CW)";
  const powerSupplyOptions = prefix === "pulsed" ? pulsedPowerSupplyOptions : cwPowerSupplyOptions;

  return (
    <Card className="bg-[var(--color-panel)]">
      <h3 className="mb-4 text-lg font-semibold">{title}</h3>
      <div className="grid gap-4 md:grid-cols-2">
        <FormField label="Cryostat">
          <OptionSelect options={cryostatOptions} register={register(`${prefix}Cryostat`)} />
        </FormField>
        <FormField label="LIV detector">
          <OptionSelect options={detectorOptions} register={register(`${prefix}LivDetector`)} />
        </FormField>
        <FormField label="Power supply">
          <OptionSelect options={powerSupplyOptions} register={register(`${prefix}PowerSupply`)} />
        </FormField>
        <FormField label="Spectrometer">
          <OptionSelect options={spectrometerOptions} register={register(`${prefix}Spectrometer`)} />
        </FormField>
        <FormField label="Spectra detector">
          <OptionSelect options={spectraDetectorOptions} register={register(`${prefix}SpectraDetector`)} />
        </FormField>
        <FormField label="Power scale"><Input type="number" step="0.1" {...register(`${prefix}PowerScale`)} /></FormField>
        <FormField label="Tmax"><Input type="number" step="0.1" {...register(`${prefix}TMax`)} /></FormField>
        <FormField label="Fmin"><Input type="number" step="0.01" {...register(`${prefix}FMin`)} /></FormField>
        <FormField label="Fmax"><Input type="number" step="0.01" {...register(`${prefix}FMax`)} /></FormField>
        <FormField label="Tfix"><Input type="number" step="0.1" {...register(`${prefix}TFix`)} /></FormField>
        {prefix === "pulsed" ? (
          <>
            <FormField label="Drive frequency (kHz)"><Input type="number" step="0.1" {...register("pulsedDriveFrequency")} /></FormField>
            <FormField label="Duty cycle (%)"><Input type="number" step="0.1" {...register("pulsedDutyCycle")} /></FormField>
            <FormField label="Gate frequency (Hz)"><Input type="number" step="0.1" {...register("pulsedGateFrequency")} /></FormField>
          </>
        ) : null}
      </div>
    </Card>
  );
}

export function MeasurementSetupForm({ mode }: { mode: "pulsed" | "cw" }) {
  const navigate = useNavigate();
  const { draft, setMeasurementSetup } = useWizard();
  const cwDefaultsFromPulsed = {
    cryostat: draft.measurementSetup.cw.cryostat || draft.measurementSetup.pulsed.cryostat,
    livDetector: draft.measurementSetup.cw.livDetector || draft.measurementSetup.pulsed.livDetector,
    spectraDetector: draft.measurementSetup.cw.spectraDetector || draft.measurementSetup.pulsed.spectraDetector,
    powerSupply: draft.measurementSetup.cw.powerSupply || draft.measurementSetup.pulsed.powerSupply,
    spectrometer: draft.measurementSetup.cw.spectrometer || draft.measurementSetup.pulsed.spectrometer,
    powerScale: draft.measurementSetup.cw.powerScale || draft.measurementSetup.pulsed.powerScale,
    tMax: draft.measurementSetup.cw.tMax ?? draft.measurementSetup.pulsed.tMax,
    fMin: draft.measurementSetup.cw.fMin ?? draft.measurementSetup.pulsed.fMin,
    fMax: draft.measurementSetup.cw.fMax ?? draft.measurementSetup.pulsed.fMax,
    tFix: draft.measurementSetup.cw.tFix ?? draft.measurementSetup.pulsed.tFix,
  };
  const form = useForm<MeasurementValues>({
    resolver: zodResolver(measurementSchema) as never,
    defaultValues: {
      pulsedCryostat: draft.measurementSetup.pulsed.cryostat,
      pulsedLivDetector: draft.measurementSetup.pulsed.livDetector,
      pulsedSpectraDetector: draft.measurementSetup.pulsed.spectraDetector,
      pulsedPowerSupply: draft.measurementSetup.pulsed.powerSupply,
      pulsedSpectrometer: draft.measurementSetup.pulsed.spectrometer,
      pulsedPowerScale: draft.measurementSetup.pulsed.powerScale,
      pulsedDriveFrequency: draft.measurementSetup.pulsed.driveFrequency,
      pulsedDutyCycle: draft.measurementSetup.pulsed.dutyCycle,
      pulsedGateFrequency: draft.measurementSetup.pulsed.gateFrequency,
      pulsedTMax: draft.measurementSetup.pulsed.tMax,
      pulsedFMin: draft.measurementSetup.pulsed.fMin,
      pulsedFMax: draft.measurementSetup.pulsed.fMax,
      pulsedTFix: draft.measurementSetup.pulsed.tFix,
      cwCryostat: cwDefaultsFromPulsed.cryostat,
      cwLivDetector: cwDefaultsFromPulsed.livDetector,
      cwSpectraDetector: cwDefaultsFromPulsed.spectraDetector,
      cwPowerSupply: cwDefaultsFromPulsed.powerSupply,
      cwSpectrometer: cwDefaultsFromPulsed.spectrometer,
      cwPowerScale: cwDefaultsFromPulsed.powerScale,
      cwTMax: cwDefaultsFromPulsed.tMax,
      cwFMin: cwDefaultsFromPulsed.fMin,
      cwFMax: cwDefaultsFromPulsed.fMax,
      cwTFix: cwDefaultsFromPulsed.tFix,
    },
  });

  const onSubmit = form.handleSubmit((values) => {
    const nextPulsed = {
      cryostat: values.pulsedCryostat ?? "",
      livDetector: values.pulsedLivDetector ?? "",
      spectraDetector: values.pulsedSpectraDetector ?? "",
      powerSupply: values.pulsedPowerSupply ?? "",
      spectrometer: values.pulsedSpectrometer ?? "",
      powerScale: values.pulsedPowerScale,
      driveFrequency: values.pulsedDriveFrequency,
      dutyCycle: values.pulsedDutyCycle,
      gateFrequency: values.pulsedGateFrequency,
      tMax: values.pulsedTMax,
      fMin: values.pulsedFMin,
      fMax: values.pulsedFMax,
      tFix: values.pulsedTFix,
    };
    const nextCw = {
      cryostat: values.cwCryostat ?? "",
      livDetector: values.cwLivDetector ?? "",
      spectraDetector: values.cwSpectraDetector ?? "",
      powerSupply: values.cwPowerSupply ?? "",
      spectrometer: values.cwSpectrometer ?? "",
      powerScale: values.cwPowerScale,
      tMax: values.cwTMax,
      fMin: values.cwFMin,
      fMax: values.cwFMax,
      tFix: values.cwTFix,
    };

    setMeasurementSetup({
      pulsed: nextPulsed,
      cw:
        mode === "pulsed"
          ? {
              ...nextCw,
              cryostat: nextCw.cryostat || nextPulsed.cryostat,
              livDetector: nextCw.livDetector || nextPulsed.livDetector,
              spectraDetector: nextCw.spectraDetector || nextPulsed.spectraDetector,
              powerSupply: nextCw.powerSupply || nextPulsed.powerSupply,
              spectrometer: nextCw.spectrometer || nextPulsed.spectrometer,
              powerScale: nextCw.powerScale || nextPulsed.powerScale,
              tMax: nextCw.tMax ?? nextPulsed.tMax,
              fMin: nextCw.fMin ?? nextPulsed.fMin,
              fMax: nextCw.fMax ?? nextPulsed.fMax,
              tFix: nextCw.tFix ?? nextPulsed.tFix,
            }
          : nextCw,
    });
    navigate(mode === "pulsed" ? "/wizard/files/pulsed-liv" : "/wizard/files/cw-liv");
  });

  return (
    <form className="grid gap-6" onSubmit={onSubmit}>
      <FieldGrid prefix={mode} register={form.register} />
      <div className="flex justify-end">
        <Button type="submit">{mode === "pulsed" ? "Continue to Pulsed Files" : "Continue to CW Files"}</Button>
      </div>
    </form>
  );
}
