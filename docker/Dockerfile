FROM debian:bullseye

RUN set -x \
 && dpkg --add-architecture i386 \
 && apt-get update \
 && apt-get install -y wget xvfb supervisor \
 && mkdir /etc/apt/keyrings/ && wget -nv -O /etc/apt/keyrings/winehq-archive.key https://dl.winehq.org/wine-builds/winehq.key \
 && wget -nv -P /etc/apt/sources.list.d/ https://dl.winehq.org/wine-builds/debian/dists/bullseye/winehq-bullseye.sources \
 && apt-get update \
 && apt-get install -y --install-recommends winehq-staging cabextract winbind unzip p7zip \
 \
 && wget -nv https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks -O /usr/local/bin/winetricks \
 && chmod +x /usr/local/bin/winetricks \
 \
 && apt-get clean \
 && apt-get autoclean \
 && apt-get autoremove \
 && rm -rf /var/lib/apt/lists/*


# Copy the NWNX4 release into the image
COPY nwnx4/ /opt/nwnx4/
COPY nwn2-reg-keys.reg /opt/

# Setup user and user-writeable dirs
RUN useradd --home-dir=/home/nwnx4 --create-home --uid=1000 nwnx4 \
 && mkdir /srv/nwnx4-userdir /srv/nwn2-home /srv/nwn2-logs \
 && chown nwnx4:nwnx4 /srv/nwnx4-userdir /srv/nwn2-home /srv/nwn2-logs

# For debugging only
# RUN set -x \
#  && apt-get update \
#  && apt-get install -y x11vnc htop iproute2 \
#  && apt-get clean \
#  && apt-get autoclean \
#  && apt-get autoremove \
#  && rm -rf /var/lib/apt/lists/*

# Default user for commands
USER nwnx4

# Configure wine
ENV WINEARCH win32

# Winetricks:
#   d3dx9_30 vcrun2005: dependencies for nwn2server
#   vcrun2019 dotnet48: dependencies for official plugins
RUN set -x \
 && mkdir -p "$HOME/Documents" \
 && ln -s /srv/nwn2-home "$HOME/Documents/Neverwinter Nights 2" \
 && mkdir -p "$HOME/.wine/drive_c/users/nwnx4/Temp/NWN2/" \
 && ln -s /srv/nwn2-logs "$HOME/.wine/drive_c/users/nwnx4/Temp/NWN2/LOGS.0" \
 \
 && xvfb-run bash -c "WINEDLLOVERRIDES='mscoree,mshtml=' wineboot && wineserver --wait" \
 && xvfb-run bash -c "winetricks -q sound=disabled d3dx9_30 vcrun2005 vcrun2019 dotnet48 && wineserver --wait" \
 && xvfb-run bash -c "wine regedit /C /opt/nwn2-reg-keys.reg && wineserver --wait" \
 \
 && rm -rf /tmp/wine* ~/.cache/wine*

# Volumes that can be bound
VOLUME ["/srv/nwnx4-userdir", "/srv/nwn2-home", "/srv/nwn2-logs", "/opt/nwn2"]
# Expose UDP port 5121 for port access
EXPOSE 5121/udp

WORKDIR /srv/nwnx4-userdir

# The entrypoint must be run as root in order to chown bound volumes
USER root
COPY supervisord.conf /etc/
COPY docker-entrypoint.sh /
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["supervisord"]

