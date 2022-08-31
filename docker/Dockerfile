FROM debian:bullseye

# Install requirements
RUN dpkg --add-architecture i386 \
 && apt-get update \
 && apt-get install -y wget xvfb wine python3 python3-pip \
 && pip install --no-cache --upgrade pip supervisor \
 \
 && cd /usr/local/bin \
 && wget https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks \
 && chmod +x winetricks \
 \
 && apt-get clean \
 && apt-get autoclean \
 && apt-get autoremove \
 && rm -rf /var/lib/apt/lists/*

# Copy the NWNX4 release into the image
COPY nwnx4/ /opt/nwnx4/

COPY supervisord.conf /etc/
COPY docker-entrypoint.sh /
COPY nwn2-reg-keys.reg /opt/

# Setup user and user-writeable dirs
RUN useradd --home-dir=/home/nwnx4 --create-home --uid=1000 nwnx4 \
 && mkdir /srv/nwnx4-userdir /srv/nwn2-home /srv/nwn2-logs \
 && chown nwnx4:nwnx4 /srv/nwnx4-userdir /srv/nwn2-home /srv/nwn2-logs

RUN chmod +x /opt/nwn2-reg-keys.reg
# Default user for commands
USER nwnx4

# Configure wine
ENV WINEARCH win32
RUN mkdir -p "$HOME/Documents" \
 && ln -s /srv/nwn2-home "$HOME/Documents/Neverwinter Nights 2" \
 && mkdir -p "$HOME/.wine/drive_c/users/nwnx4/Temp/NWN2/LOGS.0" \
 && ln -s /srv/nwn2-logs "$HOME/.wine/drive_c/users/nwnx4/Temp/NWN2/LOGS.0" \
 \
 && xvfb-run wineboot \
 && xvfb-run winetricks -q sound=disabled \
 && xvfb-run wine regedit /C /opt/nwn2-reg-keys.reg \
 && sleep 5 \
 \
 && rm -rf /tmp/wine*
# Note: The `sleep 5` command allows the wine regedit command to finish its
# job without being killed. Without it, the registry change isn't effective

WORKDIR /srv/nwnx4-userdir
# Volumes that needs to be bound
VOLUME ["/srv/nwnx4-userdir", "/srv/nwn2-home", "/srv/nwn2-logs", "/opt/nwn2"]
# Expose UDP port 5121 for port access
EXPOSE 5121/udp

# Establish entrypoint; run supervisord for tasks
ENV DISPLAY :0
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["supervisord"]
