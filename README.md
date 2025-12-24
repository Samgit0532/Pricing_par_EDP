# Pricing par EDP – Black–Scholes (C++)

Projet de pricing de produits dérivés par résolution numérique de l’EDP de Black–Scholes–Merton
à l’aide de méthodes de différences finies.

L’objectif est de construire progressivement :
- une architecture C++ propre et modulaire,
- un solver explicite puis implicite,
- le pricing de différents types d’options (européennes, puis extensions).

---

## État actuel du projet

À ce stade, le projet contient **l’architecture de base**, mais **pas encore le solver numérique**.
Le but était d’établir des fondations claires avant d’implémenter les schémas de différences finies.

Les briques déjà implémentées sont :

### 1. Modèle : `BlackScholesModel`
📁 `src/model/BlackScholesModel.hpp`

Contient les paramètres du modèle de Black–Scholes :
- taux sans risque `r`
- volatilité `sigma`
- dividende continu `q` (optionnel)

Cette classe ne fait **aucun calcul de pricing** : elle fournit simplement les paramètres du modèle
utilisés ensuite par les solveurs.

---

### 2. Grille de discrétisation : `FdGrid`
📁 `src/grid/FdGrid.hpp`

Cette classe gère la discrétisation :
- du temps : \( [0, T] \) découpé en `Nt` pas
- du prix du sous-jacent : \( [S_{min}, S_{max}] \) découpé en `Ns` pas

Elle fournit :
- les pas `dt` et `dS`,
- les grilles de temps et de prix,
- une fonction d’interpolation linéaire pour obtenir le prix pour un spot réel \( S_0 \).

---

### 3. Produits financiers

#### Interface générique
📁 `src/products/InterfaceProducts.hpp`

Définit ce qu’est une option du point de vue du solver :
- payoff terminal \( V(T,S) \),
- conditions aux bords en \( S = S_{min} \) et \( S = S_{max} \),
- possibilité d’exercice anticipé (prévu pour les options américaines).

Le solver sera écrit **uniquement** contre cette interface.

#### Options européennes
📁 `src/products/EuropeanCall.hpp`  
📁 `src/products/EuropeanPut.hpp`

Implémentations concrètes :
- Call européen
- Put européen

Chaque option :
- définit son payoff,
- définit ses conditions aux bords cohérentes avec le modèle de Black–Scholes.

---

### 4. Solver
📁 `src/solvers/`

👉 **Pour l’instant, ce dossier est vide.**

Il est réservé à l’implémentation prochaine :
- du schéma de différences finies explicite,
- puis du schéma implicite / Crank–Nicolson.

---

## Organisation des fichiers

