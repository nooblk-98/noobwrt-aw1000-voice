<h1 align="center">openwrt-aw1000-voice</h1>

<p align="center">The RJ11 phone port and SIP phones of the Arcadyan AW1000, with their own management interface</p>

<p align="center">
  <img src="https://img.shields.io/badge/router-Arcadyan%20AW1000-0a7bbb" alt="Router">
  <a href="https://openwrt.org"><img src="https://img.shields.io/badge/OpenWrt-25.12-00b5e2?logo=openwrt&logoColor=white" alt="OpenWrt"></a>
  <img src="https://img.shields.io/badge/license-GPL--2.0-blue" alt="License">
</p>

One feed that brings together the AW1000 phone stack:

- the **RJ11 phone port**: the `aw1000-fxs` driver and the `fxsd` daemon from
  [openwrt-aw1000-phone](https://github.com/NoobWRT/openwrt-aw1000-phone);
- **SIP phones and apps** on the LAN that call through the modem SIM: `msipd` from
  [openwrt-modem-sip](https://github.com/nooblk-98/openwrt-modem-sip);
- a new **management interface** on its own port, and a LuCI shortcut to it.

The management interface is for managing the phone: missed calls, the call history,
recordings and every setting. Calls are made and answered on the RJ11 phone or a SIP phone.

## Packages

| Package | What it is |
| --- | --- |
| `kmod-aw1000-fxs` | Clocks, SPI and PCM bridge for the Si32261 SLIC. Built for the exact kernel of your firmware. |
| `fxsd` | The phone port daemon: dial tone, ringing, caller ID, call waiting, DND, recording, alarms, MQTT. |
| `modem-sip` | `msipd`, a small SIP server so LAN phones and apps share the SIM with the RJ11 phone. |
| `aw1000-voice-web` | The management interface (port 7766), with the `fxs`, `modem-sip` and `aw1000-voice` rpcd backends. Replaces `luci-app-fxs` and `luci-app-modem-sip`. |
| `luci-app-aw1000-voice` | Services → Phone in LuCI: the line state and an **Open management interface** button. |

## The management interface

`http://<router>:7766/`, signed in with the router's own user name and password (rpcd).

| Page | What it does |
| --- | --- |
| Dashboard | Line state, missed calls not seen yet, calls today, quick quiet, recent calls, versions |
| Calls | Full history with filters (missed, incoming, outgoing, blocked, SIP), search, names, reasons, recordings, delete |
| Recordings | Play, save, keep or delete recordings; recording rules and space limits |
| Phonebook | Contacts with ring and DND priority, and speed dial codes |
| Blocking | Blocked numbers and ranges, hidden numbers, outgoing allow or block lists |
| Do not disturb | Quiet now, the always-ring list, weekly windows and holidays |
| Alarm calls | Wake-up rings by time and day |
| Phone services | Call forwarding and USSD codes through the modem |
| SIP phones | Turn the SIP service on, add phones, QR codes for Linphone, logins, ports, HD, VPN |
| Settings | Every `fxsd` option: caller ID, ringing, calling, history, LED, MQTT, advanced |
| Backup | Download and restore every phone setting and the call history |

It is plain HTML, CSS and JavaScript modules with no build step, under
`/www/aw1000-voice/` on the router. Restyle everything from `css/theme.css` (colors,
radius, fonts, light and dark), or edit a page in `js/pages/` and reload.

The port lives in `/etc/config/aw1000_voice`:

```sh
uci set aw1000_voice.web.port=7766
uci commit aw1000_voice
/usr/libexec/aw1000-voice-web-setup
```

The interface has its own uhttpd instance (`uhttpd.phone`), so it does not depend on
LuCI. It only answers private (RFC 1918) addresses.

## Build

The kernel module has to match your firmware's kernel build, so build these packages in the
same OpenWrt tree as the firmware. Add this repository as a feed:

```
src-git aw1000voice https://github.com/nooblk-98/openwrt-aw1000-voice.git
```

then

```sh
./scripts/feeds update aw1000voice
./scripts/feeds install -p aw1000voice -a
make package/kmod-aw1000-fxs/compile package/fxsd/compile package/modem-sip/compile \
     package/aw1000-voice-web/compile package/luci-app-aw1000-voice/compile
```

`fxsd` needs the SLIC patch file at run time (`/etc/fxs/si3226x_patch.bin`); see
[docs/hardware.md](docs/hardware.md). `modem-sip` needs `libre` and `libopus` from the
OpenWrt package feeds.

`aw1000-voice-web` and `luci-app-aw1000-voice` are architecture independent.

## Install on a running router

Remove the old LuCI pages first, they ship the same rpcd backends:

```sh
apk del luci-app-fxs luci-app-modem-sip
apk add --allow-untrusted ./aw1000-voice-web-*.apk ./luci-app-aw1000-voice-*.apk
```

## Docs

[Hardware and the SLIC patch](docs/hardware.md) · [Settings](docs/settings.md) ·
[Recording](docs/recording.md) · [SIP phones](docs/sip.md) · [SIP services](docs/sip-services.md) ·
[Scripts and MQTT](docs/scripts.md) · [Problems](docs/problems.md)

## Credits and license

GPL-2.0, see [LICENSE](LICENSE) and [CREDITS.md](CREDITS.md). The phone port and SIP
server are the work of perz3us. `msipd/src/g722.c` comes from spandsp and stays under
LGPL-2.1.
