# Objetivo

El objetivo del proyecto es aprender sobre el modelo de árboles de juego
y algoritmos básicos de solución. Trabajaremos con una versión
reducida del juego de Othello.

# Material entregado

Se entrega una representación *incompleta* de la variante 6x6 de Othello (el juego
completo tiene un tablero 8x8), y un programa principal para evaluar los algoritmos
implementados. También se incluye un artículo en cuyo apéndice
se describe la versión 6x6 de Othello y una variación principal del
mismo. Es importante leer dicho artículo para entender cómo proceder en el resto del proyecto.

# Actividad 1

Completar y verificar la representación dada del juego

# Actividad 2

Implementar los siguientes algoritmos para árboles de juego:
* Negamax sin poda alpha-beta
* Negamax con poda alpha-beta
* Scout
* Negascout = negamax con poda alpha-beta + scout

# Actividad 3

Para asegurarnos que la implementación de los algoritmos es
correcta, evaluamos los mismos a lo largo de la *variación principal*.
El valor del juego es -4 y por lo tanto, todo nodo sobre la variación
principal debe evaluar a dicho valor.

Para cada algoritmo, la evaluación se hace comenzando sobre el 
tablero terminal en la variación principal, y subimos a lo
largo de la misma. Cada vez que subimos, volvemos a ejecutar el algoritmo
de solución a partir del nodo actual hasta que termine o hasta
que el límite de tiempo expire.

El mejor algoritmo es aquel que puede llegar más lejos (arriba)
sobre la variación principal del juego.

# Entregables

Se debe entregar, en el repositorio, todo el código implementado,
los resultados experimentales, y un pequeño informe que describa lo que
hicieron y sus conclusiones.

```vega-lite
{
    "$schema":
    "https://vega.github.io/schema/vega-lite/v2.json",
    "description": "A simple bar chart with embedded data.",
    "data":{
        "values":[
            {"a":"A","b":20}
        ]
    },
    "mark": "bar",
    "encoding": {
        "x": {"field": "a", "type": "ordinal"},
        "y": {
            "field": "b", "type": "quantitative"
        },
        "width": 500,
        "height": 400
    }
}
```