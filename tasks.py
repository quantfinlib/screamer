from invoke import task
import os
import shutil
import platform

@task
def release(c, part):
    # Refresh Python --init__.py in screamer directory
    c.run("python devtools/generate_screamer__init__.py")

    """Bump version, commit, tag, and push."""
    # Bump the version
    c.run(f"bump2version {part}", pty=True)

    # Push commits
    c.run("git push origin main", pty=True)

    # Push tags
    c.run("git push origin --tags", pty=True)

@task
def test(c):

    """Build and run C++ tests in a cross-platform way."""
    # Remove build directory if it exists
    if os.path.exists('build'):
        shutil.rmtree('build')

    # Create build directory
    os.mkdir('build')

    # Change to build directory
    with c.cd('build'):

        # Run CMake configuration
        c.run('cmake .. -DCMAKE_BUILD_TYPE=Release')

        # Build the project
        if platform.system() == 'Windows':

            # On Windows, specify the build configuration (Debug/Release)
            c.run('cmake --build . --config Release')
            c.run('copy screamer_bindings*.so ..\\screamer\\')
        else:

            # On Unix-like systems
            c.run('cmake --build .')
            c.run('cp screamer_bindings*.so ../screamer/')

    # Refresh Python --init__.py in screamer directory
    c.run("python devtools/generate_screamer__init__.py")
        
    c.run('pip install -e .')
    c.run('pytest', pty=True)
    c.run('python devtools/report_baselines.py', pty=True)


@task
def docs(c):
    with c.cd('docs'):
        c.run('make clean')
        c.run('make html')        

@task(optional=['func', 'lib', 'repeat'])
def benchmark(c, func=None, lib=None, repeat=None):
    func_arg = f' --func {func}' if func else ''
    lib_arg = f' --lib {lib}' if lib else ''
    repeat_arg = f' --repeat {repeat}' if repeat else ''

    c.run(f'python benchmarks/run_benchmarks.py {func_arg}{lib_arg}{repeat_arg}', pty=True)
    c.run(f'python benchmarks/make_plots.py {func_arg}', pty=True)
    c.run(f'python benchmarks/make_rank_plot.py', pty=True)
