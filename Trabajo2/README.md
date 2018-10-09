# SRE

Tailog (_tailog.c_) es una función similar a _tail_.  Esta función debe ser capaz de leer un archivo grande (file) y filtrarlo a partir de dos condiciones:

* **time**: filtra los últimos _time_ **minutos**. Estos se cuentan desde el tiempo actual, no desde el último tiempo presente en el archivo. _Por ejemplo_:

``` time = 10, filtrará los últimos 10 minutos desde el tiempo actual. Si actualmente son las 10:10 horas, se filtrarán los datos a partir de las 10:00 horas.```

* **xip/xdom**: Permite filtrar los datos de una ip específica (_xip_) o por un dominio específico (_xdom_). Para esto se debe especificar la _ip_ o el _dom_ requerido. Este argumento es opcional.

_Para correr tailog tras compilarlo_

```./tailog file time [[xip/xdom] [ip/dom]]```

_Donde_:
* **file**: Es el archivo a filtrar
* **time**: Es el tiempo por el cual filtrar
* **xip/xdom**: Indica si se filtrará, además, por ip o por dominio respectivamente. Este argumento es opcional.
* **ip/dom**: Indica la ip o el dominio por el cual se filtrará, respectivamente. Este argumento es opcional, pero debe ir sí o sí, si se especifica el argumento anterior.

_Ejemplo:_

* Filtrar los últimos _10 minutos_ del archivo _data.csv_:

```./tailog data.csv 10```

* Filtrar por los últimos _10 minutos_ y por la ip _0.0.0.0_ del archivo _data.csv_:

```./tailog data.csv 10 xip 0.0.0.0```
