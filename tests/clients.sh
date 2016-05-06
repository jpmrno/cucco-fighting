#!/usr/bin/env bash

set -e -o pipefail
source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/concurrent.sh"

many() {
	local args=()
	local i

	for (( i = 1; i < ${1} + 1; i++ )); do
		args+=(- "Task ${i}" ./client/client.app)
	done

	concurrent "${args[@]}"
}

main() {
	if [[ -n "${1}" ]]; then
		many "${1}"
	else
		many 5
		# $(( RANDOM % 5 + 1 ))
	fi
}

main "${@}"
