#@runtime PyGhidra
#@category SonicAdventure.Export
"""Export a Ghidra candidate index and draft pseudocode; output is not verified source."""

from pathlib import Path
from ghidra.app.decompiler import DecompInterface

arguments = list(getScriptArgs())
if len(arguments) < 2:
    raise RuntimeError('Expected output root and maximum pseudocode count')
directory = Path(arguments[0]) / currentProgram.name
directory.mkdir(parents=True, exist_ok=True)
maximum = int(arguments[1])
decompiler = DecompInterface()
if not decompiler.openProgram(currentProgram):
    raise RuntimeError('Decompiler could not open the current program')

try:
    functions = currentProgram.functionManager.getFunctions(True)
    attempts = 0
    with (directory / 'function_index.tsv').open('w', encoding='utf-8', newline='') as index_file, \
            (directory / 'initial_pseudocode.c').open('w', encoding='utf-8', newline='') as code_file:
        index_file.write('address\tname\tbody_bytes\tdecompilation_status\n')
        code_file.write('/* Ghidra-generated draft only; not buildable or behavior-verified. */\n')
        while functions.hasNext() and not monitor.isCancelled():
            function = functions.next()
            status = 'indexed_only'
            if maximum == 0 or attempts < maximum:
                result = decompiler.decompileFunction(function, 15, monitor)
                attempts += 1
                if result.decompileCompleted() and result.getDecompiledFunction() is not None:
                    code_file.write('/* Address: %s */\n' % function.entryPoint)
                    code_file.write(result.getDecompiledFunction().getC())
                    status = 'draft_pseudocode'
            index_file.write('%s\t%s\t%d\t%s\n' % (
                function.entryPoint,
                function.name.replace('\t', ' ').replace('\n', ' '),
                function.body.getNumAddresses(),
                status,
            ))
    if attempts == 0:
        raise RuntimeError('No functions were exported')
    print('Exported %d ADVERTISE function candidates to %s' % (attempts, directory))
finally:
    decompiler.dispose()
