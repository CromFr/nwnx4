#!/usr/bin/env bash

set -euo pipefail

for F in /srv/{nwnx4-userdir,nwn2-home,nwn2-logs}; do
	if [ ! -w "$F" ]; then
		echo "ERROR: $F is not writeable by the user 1000 inside the container"
		echo "       Run 'sudo chown 1000:1000 /path/to/bound/volume'"
		exit 1
	fi
done

exec "$@"
