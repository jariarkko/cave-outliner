
# Cave-Outliner Software Design

## Structure

![structure](https://raw.githubusercontent.com/jariarkko/cave-outliner/main/doc/Design-Structure-Small.jpg)

## Coding Guidelines

The software is written in C++, and is open source. We collaborate at the GitHub site:

* All contributions should be made in [GitHub](https://github.com/jariarkko/cave-outliner).
* Small contributions by experienced developers can be made directly on the master branch.
* Anything large should generally be done in a separate branch, and a suitable Pull  Request be created to request its integration. This allows testing and review and modification by other contributors.
* All PRs and commits to the GitHub master branch must be made only if "make" succeeds. This will ensure tests are run and their results are correct, i.e., no existing functionality is broken.
* All code is subject to the [BSD license](https://github.com/jariarkko/cave-outliner/blob/master/LICENSE.txt).

All bugs and feature requests should be documented in GitHub issue [list](https://github.com/jariarkko/cave-outliner/issues).

## Releases

All weekly and major releases should be tagged with a tag "v(i).(j).(k)". E.g., to view current tags, use

    git tag

And making a new version:

    cd  lib
    git tag -a v2.0.0
    make updateversion
    git add psgeolib.js
    git commit -m"new version"
    git push
    git push --tags

Major versions increase the first number in the version string. New functionality should increase the second number; weekly releases and bug fixes should increase the last one.

The currently active master branch version must always be runnable; critical bugs found in it should be promptly corrected. However, the master branch version is not meant to be directly used by invidiual installations. We recommend a weekly update of live production sites, after testing and getting an agreement from main contributors that all major features seem to be fine.

## Testing

The Makefile runs various code checks and unit tests on the entire software. Its use is mandatory after changes.

Please add a test when you add a new feature that can be tested in the unit tests. Sometimes new features and tests change the expected outputs from already implemented tests. For instance, you may change the format of some outputs to be better in some way. If this happens, you need to carefully review the expected and actual outputs and make the necessary changes. Old tests are part of the software and need to be retained and maintained just like other pieces of software.

## Community behaviour

We strive for polite collaboration, welcome all contibutors, and respect the different needs diffferent people may have for the software and data formats.

We also strive for technical excellence (even if at times this means pointing out problems in the software -- but not in people).

## Detailed coding style

The following quidelines apply:

* Please comment all data structures, all functions (incl. input parameters and return values), and all complex pieces of code.
* Please use defensive programming style, i.e,. for most functions this means that all input parameters should be checked, and appropriate exceptions thrown upon error. Use "assert" extensively.
* Please indent your code appropriately, using 4 character indent settings. Please avoid the use of TABs, as their usage can sometimes be dependent on the contributor's programming environment, and look different to the other contributors.
* Avoid leaving "FIXIT" type comments in the code, as we may never find them :-) Please use the GitHub issue list instead.
* Please limit the use of external libraries to the minimum; we attempt to keep cave-outliner as portable and independent of other systems as possible.

