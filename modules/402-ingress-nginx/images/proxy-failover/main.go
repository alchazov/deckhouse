/*
Copyright 2023 Flant JSC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package main

import (
	"log"
	"os"

	"golang.org/x/sys/unix"
)

func main() {
	controllerName := os.Getenv("CONTROLLER_NAME")
	if len(controllerName) == 0 {
		log.Fatal("CONTROLLER_NAME env is empty")
	}

	nginxConfTemplateBytes, err := os.ReadFile("/etc/nginx/nginx.conf.tpl")
	if err != nil {
		log.Fatal(err)
	}

	nginxConfTemplate := os.ExpandEnv(string(nginxConfTemplateBytes))

	err = os.WriteFile("/etc/nginx/nginx.conf", []byte(nginxConfTemplate), 0666)
	if err != nil {
		log.Fatal(err)
	}

	err = unix.Exec("/usr/sbin/nginx", []string{"nginx", "-g", "daemon off;"}, os.Environ())
	if err != nil {
		log.Fatal(err)
	}
}
