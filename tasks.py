from invoke import task
import subprocess

@task
def release(c, part):
    """Bump version, commit, tag, and push."""
    # Bump the version
    c.run(f"bump2version {part}", pty=True)

    # Push commits
    c.run("git push origin main", pty=True)

    # Push tags
    c.run("git push origin --tags", pty=True)
