#pragma once

/*CAUTION&&TROUBLE SHOOTING *
* * * * * * * * * * * * * * *
* + save all files before git
*   => Shift+Ctrl+S in VS
*/

/* * * * * * * * *
 * Git Manual    *
 * * * * * * * * */

/*create a new repository *
* * * * * * * * * * * * * *
* 1. add a README file
* 2. choose a license
*   => Apache License 2.0
* 3. create repository
*/

/*make .gitignore file *
* * * * * * * * * * * *
* 1. open VS Code (or make .gitignore file with txt in solution folder and jump to 4th step)
* 2. new file
* 3. save as ".gitignore" in solution folder
* 4. type "
*   # Directories
*   .vs/
*   x64/
*   DirectX3D11/x64/
*   Debug/
*   DirectX3D11/Debug/
*   bin/
*   bin-int/
*
*   # Files
*   *.user
*   "
*   - DirectX3D11 = entry project folder
* 5. save .gitignore file
*/

/*open cmd *
* * * * * *
* 1. open solution folder
* 2. right click directory path
*   => edit address
*   => type "cmd"
*/

/*git clone *
* * * * * * *
* 1. type "git clone https://github.com/GyumLee/DirectX3D11 Git"
*   - GyumLee = github username
*   - DirectX3D11 = repogitory name
*   - Git = folder name to fetch
* 2. copy files in Git folder
*   - .git
*   - LICENSE
*   - README.md
* 3. paste files in solution folder
* 4. delete Git folder
*/

/*git status *
* * * * * * *
* + type "git status"
*/

/*git add *
* * * * * *
* + type "git add *"
*/

/*git reset *
* * * * * * *
* + type "git reset ."
*/

/*git commit *
* * * * * * *
* + type "git commit -m "desc""
    - m = message
*   - desc = description
*/

/*git push *
* * * * * *
* + type "git push"
* + type "git push origin main"
*   - main = branch = master
*/

/*git fetch *
* * * * * * *
* + type "git fetch"
*/

/*git pull *
* * * * * * *
* + type "git pull"
*/

/*git version *
* * * * * * *
* + type "git --version"
*/