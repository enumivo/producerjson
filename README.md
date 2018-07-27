# producerjson

Producer JSON smart contract for Enumivo.

# Usage

### View the table

```
enucli get table producerjson producerjson producerjson
```

### Add to the table

```
enucli push action producerjson set '{"owner":"your_account", "json": "your_json"}' -p your_account@active
```

**Example**:
```
enucli push action producerjson set '{"owner":"teamgreymass", "json": "'`printf %q $(cat bp.json | tr -d "\r")`'"}' -p teamgreymass@active
```

### Remove from the table

```
enucli push action producerjson del '{"owner":"your_account"}' -p your_account@active
```


---

# How to build
```
./build.sh
```
or
```
enumivocpp -g producerjson.abi producerjson.cpp && enumivocpp -o producerjson.wast producerjson.cpp
```

# How to deploy

```
enucli set contract producerjson producerjson -p producerjson@active
```
