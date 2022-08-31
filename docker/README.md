

# Build the docker image

1. Download and extract a nwnx4 release zip (or build it yourself), and install it the `docker/nwnx4` directory
2. Run `docker build . --tag nwnx4`

# Run the container

1. Get a copy of the NWN2 install folder. You can optionally follow [these
   instructions](https://nwn.wiki/display/NWN2/Server+setup#Serversetup-Method2:lightserverinstall)
   to reduce the size of the install folder (Warning: it removes official
   campaigns content).
2. Start the container:
```bash
# Customize the paths on the host
NWNX4_USERDIR="$PWD/nwnx4-userdir" # Custom NWNX4 files
NWN2_HOME="$PWD/nwn2-home" # Equivalent of Documents/Neverwinter Nights 2, where your custom content is located
NWN2_INSTALL="$PWD/nwn2" # NWN2 install folder

# Create the directories
mkdir -p "$NWNX4_USERDIR" "$NWN2_HOME"
# Make sure it's writable by user 1000 (mapped to the nwnx user in the container)
chown 1000:1000 "$NWNX4_USERDIR" "$NWN2_HOME"

# Run the container
docker run -it --name=my_nwnx4 \
	-v "$NWNX4_USERDIR:/srv/nwnx4-userdir" \
	-v "$NWN2_HOME:/srv/nwn2-home" \
	-v "$NWN2_INSTALL:/opt/nwn2:ro" \
	-p 5121:5121/udp \
	nwnx4:latest
```
3. If NWNX4_USERDIR or NWN2_HOME were empty, they should now contain example
   files and useful folders that you should customize to configure NWNX4 and
   your server. Restart the container after making changes
