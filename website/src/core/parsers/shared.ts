export function splitNumericFields(line: string) {
  if (line.includes(",")) {
    return line.split(",").map((entry) => entry.trim()).filter(Boolean);
  }

  return line.split(/\s+/).map((entry) => entry.trim()).filter(Boolean);
}
