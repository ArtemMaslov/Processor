﻿# Processor
Данный проект эмулирует процессор, который основан на стеке, имеет регистры и оперативную память,
 умеет производить базовые математические вычисления. В проект также входят ассемблер и дизассемблер,
 которые позволяют переводить ассемблерный код в исполняемый для процессора и наоборот.

1. [Использование.](#использование.)
2. [Описание структуры процессора.](#описание-структуры-процессора.)
3. [Синтаксис ассемблера.](#синтаксис-ассемблера.)

## Использование.

1. Создать папку, в которой будет лежать код на ассемблере `asm.code`, ассемблер и процессор.
2. Запустить ассемблер. На выходе получится файл с исполняемым кодом `code.asm`.
3. Запустить процессор.
4. В результате будет выполнена программа, написанная на ассемблере.

## Описание структуры процессора.

### Исходный код для ассемблера.
Код представляет собой текст в кодировке Windows-1251. Каждая команда должна начинаться с новой строки. Точка запятая `;` является комментарием, поэтому текст 
после неё будет игнорироваться ассемблером.

### Стек.
Стек представляет собой динамический массив чисел с плавающей запятой удвоенной точности `double`.
Добавлять числа можно только в конец стека (push x). Извлекать числа из стека можно только с конца (pop x).

### Оперативная и видео память.
Память процессора представляет собой массив фиксированной на этапе компиляции длины. Доступ к ней можно получить указав квадратные скобки `[]` в командах `push` и `pop`.
Первые 128 элементов являются оперативной памятью, остальные элементы являются видео памятью.

Экран процессора имеет размер 60 на 60 пикселей. Вывести его можно в консоль командой `dsp`, 
`.` будет означать не горящий пиксель, `*` будет означать горящий пиксель.
Видео память начинается со 128 адреса оперативной памяти. 1 бит кодирует один пиксель. Поэтому, чтобы переключить состояние пикселя с координатами `(x; y)`, 
нужно вычислить его смещение по формуле `offset = 128 + x + 60 * y` и выполнить команду `push [offset]`. Из стека будет взято число, если оно не равно 0, то пиксель будет загорится, иначе потухнет.

При обращении к памяти, адреса округляются до целых путём отбрасывания дробной части, поэтому следующие команды идентичны:
```
push [5]
push [5.1]
push [5.98]
```

### Переходы и метки.
Переходы бывают обычные `jmp` и условные `ja, je, jae, jbe, jne, je`. Обычные переходы изменяют порядок выполнения инструкций процессором. Условные переходы извлекают два числа из стека,
 и если они удовлетворяют определённому соотношению, то переход выполняется. Вне зависимости от совершения перехода, эти числа назад в стек не возвращаются. 
После команды перехода указывается метка, к которой нужно совершить переход в коде: `jmp label_name`. Имя метки `label_name` может содержать любые символы, кроме пробела,
 символа новой строки, знака табуляции и двоеточия. Двоеточие стоящее в конце имени метки будет игнорироваться, поэтому следующие команды будут равнозначны:
```
jmp 1_label_1
jmp 1_label_1:
```

Примеры корректных и некорректных имён меток:
```
jmp 1 label	              ; Bad
jmp 1_label                   ; Ok
jmp 1-label                   ; Ok
jmp 1
label                         ; Bad
jmp !@#$%^&*()_+'"<>,.?/{}[]  ; Ok
```

Чтобы указать в какое место должен быть совершен переход, в коде должна встречаться метка:
```
label:
```
Имя метки должно быть уникальным.

### Функции.
Функции похожы на безусловные переходы, но имеет возвожность вернуться к месту вызова `ret`. Синтаксис: `call label`, где `label` - это имя метки, к которой нужно совершить переход.
В момент вызова функции в стек добавляется адрес возврата.
Чтобы вернуться к выполнению инструкций после вызова функции, в ней нужно вызвать команду `ret`, при этом этом адресом возврата считается последнее число, лежащее в стеке. 
То есть, если функции в время работы добавляла какие-то данные в стек, то перед возвратом функция должна сама удалить эти данные из стека, иначе поведение не определено. 
Если функция не содержит команду `ret`, то процессор продолжит выполнять команды, расположенные сразу после функции, при этом адрес возврата из стека удалён не будет.

## Синтаксис ассемблера.

1. `push <args>` - добавить число в конец стека. Возможны следующие варианты использования:

```
push 5.2       // Добавить в стек число 5.2.
push ax        // Добавить в стек число из регистра ax.
push 6 + dx    // Добавить в стек число, равное сумме 6 и числа из регистра dx.
push [2]       // Добавить в стек число из 2 ячейки оперативной памяти.
push [ax]      // Добавить в стек число из ячейки оперативной памяти с адресом ax.
push [50 + ax] // Добавить в стек число из ячейки оперативной памяти с адресом (50 + ax).
```

2. `pop <args>` - удалить число из конца стека. Возможны следующие варианты использования:

```
pop           // Удалить последнее число из стека.
pop ax        // Удалить последнее число из стека и положить его в регистр ax.
pop [2]       // Удалить последнее число из стека и положить его в 2 ячейку оперативной памяти.
pop [ax]      // Удалить последнее число из стека и положить его в ячейку оперативной памяти с адресом ax.
pop [50 + ax] // Удалить последнее число из стека и положить его в ячейку оперативной памяти с адресом (50 + ax).
```

3. `in` - прочитать число из консоли и добавить его в конец стека.
4. `out` - удалить последнее число из стека и вывести его на экран.
5. `add` - взять два числа из стека, найти их сумму и результат положить в стек.
6. `sub` - взять два числа из стека, найти их разность и результат положить в стек. 
Уменьшаемым является предпоследнее  в стеке число. Вычитаемым является последнее в стеке число.
7. `mul` - взять два числа из стека, найти их произведение и результат положить в стек.
8. `div` - взять два числа из стека, найти их частное и результат положить в стек. 
Делимым является предпоследнее  в стеке число. Делителем является последнее в стеке число.
9. `hlt` - завершение программы.
10. `jmp <label>` - выполнить переход к указанной метке.
11. `ja <label>` - выполнить переход к указанной метке, если предпоследнее число в стеке больше последнего.
12. `jae <label>` - выполнить переход к указанной метке, если предпоследнее число в стеке больше или равно последнему.
13. `jb <label>` - выполнить переход к указанной метке, если предпоследнее число в стеке меньше последнего.
14. `jbe <label>` - выполнить переход к указанной метке, если предпоследнее число в стеке меньше или последнему.
15. `je <label>` - выполнить переход к указанной метке, если предпоследнее число в стеке равно последнему.
16. `jne <label>` - выполнить переход к указанной метке, если предпоследнее число в стеке не равно последнему.
17. `call <label>` - выполнить переход к указанной метке с записью адреса возврата в стек (вызвать функцию).
18. `ret` - выполнить выход из функции.
19. `dsp` - вывести изображение, хранящееся в видео-памяти на экран.
20. `cos` - взять число из стека, посчитать косинус этого числа и результат положить обратно в стек.
21. `sin`- взять число из стека, посчитать синус этого числа и результат положить обратно в стек.
22. `sqrt` - взять число из стека, извлечь квадратный корень из этого числа и результат положить обратно в стек.
23. `int` - взять число из стека, отбросить его дробную часть и результат положить обратно в стек.
