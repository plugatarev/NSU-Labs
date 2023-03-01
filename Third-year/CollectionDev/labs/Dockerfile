FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y \
        openssh-client \
        git \
        wget \
        libxml2 \
        libxslt1-dev \
        libpcre3 \
        libpcre3-dev \
        zlib1g \
        zlib1g-dev \
        openssl \
        libssl-dev \
        libtool \
        automake \
        gcc \ 
        make \
	systemctl	
        

RUN wget "https://nginx.org/download/nginx-1.23.3.tar.gz" && \
    tar -xzvf nginx-1.23.3.tar.gz && \
    cd nginx-1.23.3 && \
    ./configure --sbin-path=/usr/bin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --with-pcre --pid-path=/var/run/nginx.pid --with-http_ssl_module && \
    make && \
    make install

RUN rm /usr/local/nginx/html/index.html
COPY index.html /usr/local/nginx/html/index.html
   
EXPOSE 80


CMD ["nginx", "-g", "daemon off;"]
  

    
    
