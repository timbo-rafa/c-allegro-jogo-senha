## Compilation

```
cc jogo-senha.c -o jogo-senha $(pkg-config  allegro --libs --cflags)
```

`$(pkg-config allegro --libs --cflags)`  resolves to `-lalleg` in `Ubuntu 16.04`

## Running

```
./jogo-senha
```
