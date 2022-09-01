#!/usr/bin/env bash

set -euo pipefail

if [[ "$(whoami)" == root ]]; then
	for F in /srv/{nwnx4-userdir,nwn2-home,nwn2-logs}; do
		chown nwnx4:nwnx4 "$F"
		chmod 755 "$F"
	done
fi

exec "$@"
