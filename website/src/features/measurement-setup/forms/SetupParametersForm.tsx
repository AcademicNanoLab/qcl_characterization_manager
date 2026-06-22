import { zodResolver } from "@hookform/resolvers/zod";
import { useForm } from "react-hook-form";
import { useNavigate } from "react-router-dom";
import type { z } from "zod";
import { FormField } from "@/components/forms/FormField";
import { Button } from "@/components/ui/button";
import { Card } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { Select } from "@/components/ui/select";
import { useWizard } from "@/features/wizard/context/WizardContext";
import { setupSchema } from "./schemas";

type SetupFormValues = z.infer<typeof setupSchema>;

export function SetupParametersForm() {
  const navigate = useNavigate();
  const { draft, setProjectMetadata } = useWizard();
  const form = useForm<SetupFormValues>({
    resolver: zodResolver(setupSchema) as never,
    defaultValues: {
      author: draft.projectMetadata.author,
      date: draft.projectMetadata.date,
      waferNumber: draft.projectMetadata.device.waferNumber,
      deviceName: draft.projectMetadata.device.deviceName,
      design: draft.projectMetadata.device.design,
      waveguide: draft.projectMetadata.device.waveguide,
      approxEmissionFrequencyThz: draft.projectMetadata.device.approxEmissionFrequencyThz,
      width: draft.projectMetadata.device.dimensions.width,
      length: draft.projectMetadata.device.dimensions.length,
      height: draft.projectMetadata.device.dimensions.height,
    },
  });

  const onSubmit = form.handleSubmit((values) => {
    setProjectMetadata({
      author: values.author,
      date: values.date,
      device: {
        waferNumber: values.waferNumber,
        deviceName: values.deviceName,
        design: values.design,
        waveguide: values.waveguide,
        approxEmissionFrequencyThz: values.approxEmissionFrequencyThz,
        dimensions: {
          width: values.width,
          length: values.length,
          height: values.height,
        },
      },
    });
    navigate("/wizard/measurement-pulsed");
  });

  return (
    <Card>
      <form className="grid gap-5 md:grid-cols-2" onSubmit={onSubmit}>
        <FormField label="Author"><Input {...form.register("author")} /></FormField>
        <FormField label="Date"><Input type="date" {...form.register("date")} /></FormField>
        <FormField label="Wafer number"><Input {...form.register("waferNumber")} /></FormField>
        <FormField label="Device name"><Input {...form.register("deviceName")} /></FormField>
        <FormField label="Design">
          <Select {...form.register("design")}>
            <option value="N/A">N/A</option>
            <option value="BTC">BTC</option>
            <option value="LO phonon">LO phonon</option>
            <option value="Hybrid">Hybrid</option>
            <option value="Other">Other</option>
          </Select>
        </FormField>
        <FormField label="Waveguide">
          <Select {...form.register("waveguide")}>
            <option value="SM">SM</option>
            <option value="DM">DM</option>
          </Select>
        </FormField>
        <FormField label="Approx. emission frequency (THz)">
          <Input type="number" step="0.01" {...form.register("approxEmissionFrequencyThz")} />
        </FormField>
        <FormField label="Width (um)"><Input type="number" step="0.1" {...form.register("width")} /></FormField>
        <FormField label="Length (mm)"><Input type="number" step="0.1" {...form.register("length")} /></FormField>
        <FormField label="Height (um)"><Input type="number" step="0.1" {...form.register("height")} /></FormField>
        <div className="md:col-span-2 flex justify-end">
          <Button type="submit">Continue to Measurement Setup</Button>
        </div>
      </form>
    </Card>
  );
}
