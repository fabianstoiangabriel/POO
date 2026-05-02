# Simulare Neuronala Hodgking-Huxley


# Surse de documentare si insppiratie pentru codat si formulele matematice

chrome-extension://efaidnbmnnnibpcajpcglclefindmkaj/https://www.math.mcgill.ca/gantumur/docs/reps/RyanSicilianoHH.pdf
chrome-extension://efaidnbmnnnibpcajpcglclefindmkaj/https://seti.net/Neuron%20Lab/NeuronReferences/Neuroscience%20-%20Bear.pdf
https://github.com/Daniel-M/Hodgking-Huxley
https://www.youtube.com/watch?v=8-B6ryuBkCM
https://www.youtube.com/watch?v=OSAOh4L41Wg
https://www.youtube.com/watch?v=E_-lMZDi7Uw
https://www.youtube.com/watch?v=ukNbG7muKho
https://neuronaldynamics.epfl.ch/online/Ch2.S2.html
https://stackoverflow.com/questions
https://www.geeksforgeeks.org/

## Ce face acest proiect

Simulează cum funcționează creierul la nivel electric. Mai concret: modelează cum niște celule (neuroni) se "aprind" și trimit semnale unele altora, exact cum o fac în creierul unui om sau animal.

Modelul matematic din spate se numește **Hodgkin-Huxley** 

##Cum functioneaza

 Un neuron e conectat la mai multe  canalele ionice. Fiecare canal lasă să treacă un alt tip de particule încărcate electric (ioni). Când suma curenților depășește un prag, tensiunea "explodează" brusc  asta se numește **potențial de acțiune**


[Curent extern] ──► [Neuron = condensator] ──► tensiune V(t)
                          │
              ┌───────────┼───────────┐
              │           │           │
           Na+ intră   K+ iese    Leak (scurgere)
           (depolarizare) (repolarizare) (echilibru)


## Structura proiectului


.
├── SystemErrors.h      # Excepții custom
├── Gate.h              # Porti moleculare de activare
├── Channel.h           # Canale ionice (grupuri de porți)
├── Neuron.h            # Celula nervoasă propriu-zisă
├── Synapse.h           # Conexiunea dintre doi neuroni
├── NeuralNetwork.h     # O rețea de neuroni + sinapse
├── Amygdala.h          # Rețea specializată (modelul amigdalei)
└── neural_system.cpp   # main() 



### `SystemErrors.h` 

Nimic special din punct de vedere biologic. Trei excepții custom, toate derivate din `SimulationException`:

| Excepție | Când apare |
|---|---|
| `InvalidConductanceException` | Conductanță negativă (fizic imposibil) |
| `InvalidVoltageException` | Tensiune în afara intervalului `[-100, 50]` mV |
| `TimestepException` | Pas de timp `dt > 0.1` ms (simularea devine instabilă) |

---

### `Gate.h` — Porțile moleculare

**Ce e o poartă?** Fiecare canal ionic are "supape" moleculare care se deschid și se închid în funcție de tensiunea membranei. O poartă are o singură variabilă de stare `P ∈ [0, 1]`, unde `0` = complet închisă, `1` = complet deschisă.

**Ecuația din `updD()`** este o ecuație diferențială ordinară simplă (Euler explicit):

```
dP/dt = α(V) · (1 - P) - β(V) · P
```

- `α(V)` — rata de deschidere, depinde de tensiunea `V`
- `β(V)` — rata de închidere, depinde de tensiunea `V`

Sunt implementate 5 tipuri de porți:

| Clasă | Rol biologic | Folosită în |
|---|---|---|
| `MGate` | Activare rapidă Na⁺ | `SodiumChannel` |
| `HGate` | Inactivare Na⁺ | `SodiumChannel` |
| `NGate` | Activare K⁺ | `PotassiumChannel` |
| `CaMGate` | Activare Ca²⁺ | `CaChannel` |
| `CaHGate` | Inactivare Ca²⁺ | `CaChannel` |

> 
---

### `Channel.h` — Canalele ionice

**Ce e un canal?** Un grup de porți care împreună controlează curentul unui singur tip de ion prin membrană. Formula generală a curentului:

```
I = g_ion · (produs al porților deschise) · (V - E_ion)
```

- `g_ion` — conductanța maximă (cât de "larg" e canalul când e complet deschis)
- `E_ion` — potențialul de echilibru al ionului (tensiunea la care curentul net este zero)
- `(V - E_ion)` — forța motoare

Sunt implementate 4 canale:

| Clasă | Ion | Formula curentului | Rol |
|---|---|---|---|
| `SodiumChannel` | Na⁺ | `g · m³ · h · (V - E)` | Depolarizare rapidă |
| `PotassiumChannel` | K⁺ | `g · n⁴ · (V - E)` | Repolarizare |
| `leakChannel` | mix | `g · (V - E)` | Curent de fond, menține echilibrul |
| `CaChannel` | Ca²⁺ | `g · m² · h · (V - E)` | Modulare, plasticitate |




---

### `Neuron.h`

**Ce face un neuron?** Integrează toți curenții ionici + curentul extern și actualizează tensiunea membranei `V` la fiecare pas de timp.

**Ecuația principală** (legea lui Kirchhoff aplicată membranei):

```
C_m · dV/dt = I_ext - Σ I_ion
```

Implementat în `update(double dt)` ca integrare Euler:

```
double dV = (I_ext - I_ion) / C_m;
V += dV * dt;
```

**Parametri impliciți** (valorile clasice Hodgkin-Huxley):

| Parametru | Valoare | Semnificație |
|---|---|---|
| `C_m` | `1.0 µF/cm²` | Capacitatea membranei |
| `V` initial | `-65.0 mV` | Potențialul de repaus |
| `I_ext` | `0.0 µA/cm²` | Curent extern (stimul) |

**`neuronCount`** este `static` — contorizează toți neuronii creați vreodată în program (nu cei activi curent).



---

### `Synapse.h`

**Ce e o sinapsă?** Conexiunea direcționată dintre doi neuroni: `pre → post`. Când neuronul `pre` se activează (tensiunea depășește `0 mV`), după o întârziere `synapticDelay`, injectează un curent în neuronul `post`.

```
Neuron PRE ──[strength]──► Neuron POST
```

- `strength > 0` → sinapsă **excitatorie** (împinge POST spre activare)
- `strength < 0` → sinapsă **inhibitorie** (împiedică POST să se activeze)

**Cum funcționează `transmit()`:**

```
if (V_pre > 0 && releaseTime < 0)      // detectează spike-ul
    releaseTime = time + synapticDelay; // programează eliberarea
if (time >= releaseTime && releaseTime >= 0) { // a sosit momentul
    post->setExternalCurrent(strength); // injectează curent
    releaseTime = -1.0;                 // resetează
}
```


### `NeuralNetwork.h` 

Container pentru `Neuron*` și `Synapse*`

**`update(double dt)`** — un singur pas de timp:
1. Toate sinapsele transmit (verifică dacă neuronul pre s-a activat)
2. Toți neuronii se actualizează (calculează noua tensiune)
3. Se calculează tensiunea medie a populației → se adaugă în `eegSignal`

**`eegSignal`** simulează un **EEG** (electroencefalogramă) în realitate, EEG-ul măsoară media activității electrice a milioane de neuroni. Aici se face același lucru, la scară mică.


### `Amygdala.h`

**Ce e amigdala?** O structură din creier implicată în procesarea fricii și emoțiilor. Modelul de aici e o simplificare: 80% neuroni excitatori + 20% inhibitori, conectați complet între ei.

```
Amygdala(100 neuroni):
  ├── 80 excitatori  ──(+0.5)──► toți ceilalți excitatori
  └── 20 inhibitori  ──(-0.5)──► toți excitatorii
```

**`setFearState(bool fear)`:**
- `fear = true` → curent extern `15.0` µA/cm² (activitate intensă)
- `fear = false` → curent extern `2.0` µA/cm² (activitate de fond)

---

### `neural_system.cpp` — `main()`

```
Amygdala amygdala(100);          // 100 neuroni, conectați automat
amygdala.setFearState(true);      // simulează "stare de frică"
// rulează 1000 pași × dt=0.01ms = 10ms de activitate neurală
for (int i = 0; i < 1000; i++)
    amygdala.update(0.01);
// afișează semnalul EEG la fiecare 100 pași (1ms)
```
