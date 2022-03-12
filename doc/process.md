# OpenEVSE Process Documentation

## Change management

Change requests and bug reports are are submitted through the [GitHub issue tracker](https://github.com/OpenEVSE/open_evse/issues). There should be a single issue/bug/feature per ticket.

All changes should be made on a branch or fork of the `stable` branch and the change should be submitted through a GitHub pull request. Pull requests will be reviewed by a repository administrator before being merged into the `stable` branch. For changes by the repository administrator, the pull request should be reviewed by another administrator before being merging.

Changes from the [development fork](https://github.com/lincomatic/open_evse) will be merged into the `stable` branch after being reviewed by the repository administrator.

## Building

Instructions on building the firmware an be found in [OpenEVSE Firmware Loading](../firmware/open_evse/LoadingFirmware.md).

For releases and PR approval the builds are built using the [Build/Release OpenEVSE](https://github.com/OpenEVSE/open_evse/actions/workflows/build.yml) workflow to ensure consistent behaviour.

## Testing

See the OpenEVSE [Testing Basic and Advanced](https://openevse.dozuki.com/Guide/Testing+Basic+and+Advanced/12?lang=en) guide.

## Creating a new Releases

1. Ensure GitHub actions are complete and green
    [![Build/Release OpenEVSE](https://github.com/OpenEVSE/open_evse/actions/workflows/build.yml/badge.svg)](https://github.com/OpenEVSE/open_evse/actions/workflows/build.yml)
1. Check the [version number](https://semver.org/) is correct
1. Go to the latest [Development Build](https://github.com/OpenEVSE/open_evse/releases/tag/latest) release
1. Edit the release
1. Change the tag from latest to the [version number](https://semver.org/), for example: `v1.0.0`
1. Change the branch to `latest`, is not automatically filled in so have to type the name in. Will show the appropriate commit hash on pressing enter
1. Enter the version number as the release name, for example: `v1.0.0`
1. Add the release notes, a good start is to use the `Auto-Generated Release Notes` and edit as needed
1. Remove any unwanted binaries, need at least:
   - `emonevse.hex`
   - `emonevse_3ph.hex`
   - `openevse.hex`
   - `openevse_eu.hex`
1. Unselect the `Pre-release` checkbox
1. Click `Update release`
1. Ensure the release validation action is green
    [![Release Validation](https://github.com/OpenEVSE/open_evse/actions/workflows/release_validation.yaml/badge.svg)](https://github.com/OpenEVSE/open_evse/actions/workflows/release_validation.yaml)
