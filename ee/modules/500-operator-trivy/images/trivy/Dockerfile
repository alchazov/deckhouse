ARG BASE_ALPINE
ARG BASE_GOLANG_19_BULLSEYE

FROM $BASE_GOLANG_19_BULLSEYE AS build
ARG TRIVY_VERSION=v0.44.0-flant
ARG TRIVY_DB_VERSION=flant-latest
ARG SOURCE_REPO
ENV SOURCE_REPO=${SOURCE_REPO}

WORKDIR /src
RUN git clone --depth 1 --branch ${TRIVY_DB_VERSION} ${SOURCE_REPO}/aquasecurity/trivy-db.git && \
    git clone --depth 1 --branch ${TRIVY_VERSION} ${SOURCE_REPO}/aquasecurity/trivy.git && \
    cd trivy && \
    CGO_ENABLED=0 GOOS=linux go build -ldflags '-s -w -extldflags "-static"' -o trivy ./cmd/trivy/main.go

FROM $BASE_ALPINE
COPY --from=build /src/trivy /usr/local/bin/
ENTRYPOINT [ "/usr/local/bin/trivy" ]
ENV LANG=C.UTF-8
