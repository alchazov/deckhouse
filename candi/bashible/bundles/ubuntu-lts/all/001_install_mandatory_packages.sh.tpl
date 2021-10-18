# Copyright 2021 Flant JSC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

SYSTEM_PACKAGES="curl wget virt-what inotify-tools bash-completion lvm2 parted apt-transport-https sudo nfs-common"
KUBERNETES_DEPENDENCIES="iptables iproute2 socat util-linux mount ebtables ethtool conntrack"

# Hack for Ubuntu 16.04
if bb-is-ubuntu-version? 16.04 ; then
  if grep -q "^mozilla\/DST_Root_CA_X3.crt$" /etc/ca-certificates.conf; then
    sed -i "/mozilla\/DST_Root_CA_X3.crt/d" /etc/ca-certificates.conf
    update-ca-certificates --fresh
  fi
fi

bb-apt-install ${SYSTEM_PACKAGES} ${KUBERNETES_DEPENDENCIES}

bb-rp-install "jq:1.6"
