FROM php:7.2-apache

RUN apt-get update \
  && apt-get install -y libpng-dev \
  && docker-php-ext-install gd

COPY vendor /var/www/vendor
