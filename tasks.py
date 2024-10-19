from invoke import task
import os
import shutil
import platform

@task
def release(c, part):
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
        c.run('cmake ..')

        # Build the project
        if platform.system() == 'Windows':
            # On Windows, specify the build configuration (Debug/Release)
            c.run('cmake --build . --config Debug')
            c.rum('copy screamer_bindings*.so ..\\screamer\\')
        else:
            # On Unix-like systems
            c.run('cmake --build .')
            c.run('cp screamer_bindings*.so ../screamer/')

    c.run('pytest')
