$_public_branch = "dev"
$_private_branch = "main"
git show-ref --verify --quiet "refs/heads/$_private_branch"
if ($LASTEXITCODE -ne 0) {
    Write-Error “Source branch '$_private_branch' is not exists.”
}
if ($args.Length -eq 0) {
    Write-Error "No commit message provided."
}
$_commit_message = $args[0]
$_git = (Get-Command "git" -ErrorAction SilentlyContinue).Path
if ($_git.Length -eq 0) {
    Write-Error "No git installed, or git is not included in PATH."
}
git diff --quiet
if ($LASTEXITCODE -ne 0) {
    Write-Error "Worktree is not clean, commit all changes first."
}
$_current_branch = git rev-parse --abbrev-ref HEAD
git show-ref --verify --quiet "refs/heads/$_public_branch"
if ($LASTEXITCODE -ne 0) {
    Write-Host "Target branch '$_public_branch' is not exists, creating new orphan branch."
    git checkout --orphan $_public_branch
    git reset
} else {
    if ($_current_branch -ne $_public_branch) {
        git checkout $_public_branch
    }
    git merge --squash --allow-unrelated-histories --strategy-option=theirs $_private_branch
}
git add .
git commit -m "$_commit_message"
if ($_current_branch -ne $_public_branch) {
    git checkout $_current_branch
}

