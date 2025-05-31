# 🎮 Wolf3D - Projet Epitech

<div align="center">

[![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CSFML](https://img.shields.io/badge/CSFML-8CC445?style=for-the-badge&logo=sfml&logoColor=white)](https://www.sfml-dev.org/)
[![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)](https://www.linux.org/)

</div>

## 📝 Description

**Wolf3D** est un jeu de tir à la première personne inspiré du légendaire Wolfenstein 3D. Ce projet utilise la technique du **raycasting** pour créer un environnement 3D immersif avec une ambiance unique inspirée des **Backrooms** 🏚️. 

L'atmosphère oppressante des couloirs jaunes, les ennemis qui rôdent dans l'ombre créent une expérience de jeu unique mêlant FPS classique et horreur psychologique.

## ✨ Fonctionnalités

### 🏗️ Moteur de jeu
- 🎨 **Raycasting** avec rendu de murs texturés style Backrooms
- 🟡 **Textures** inspirées des couloirs jaunes iconiques
- 💡 **Système d'éclairage** avec lampe torche dynamique (indispensable dans les Backrooms)
- 📊 **Affichage FPS** en temps réel

### 🎯 Gameplay
- 🔫 **Système d'armes** avec animations fluides
- 👾 **Ennemis intelligents** avec IA de patrouille et de combat
- ❤️ **Système de santé** avec indicateur visuel
- 💾 **Système de sauvegarde** automatique
- 🗺️ **Plusieurs niveaux** progressifs

### 🎮 Interface
- 📋 **Menu principal** interactif
- ⚙️ **Menu des paramètres** avec :
  - 🔊 Réglage du volume
  - 🖥️ Changement de résolution
- 🎯 **Viseur personnalisé** 

## 🚀 Installation

### Prérequis

```bash
# 🐧 Sous Linux (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libcsfml-dev
```

### Cloner le projet

```bash
# 📥 Cloner le repository (HTTPS)
git clone https://github.com/DenizEpitech/Backroom3D.git

# 📥 Ou avec SSH (recommandé)
git clone git@github.com:DenizEpitech/Backroom3D.git

# 📂 Entrer dans le dossier
cd Backroom3D
```

### Compilation

```bash
# 🔨 Compiler le projet
make

# 🧹 Nettoyer les fichiers temporaires
make clean

# 🔄 Recompiler entièrement
make re
```

## 🎮 Comment jouer

### Lancer le jeu

```bash
# 🏃 Exécuter avec une map
./wolf3d map/floor

# 📋 Maps disponibles :
./wolf3d map/floor       # Niveau 1
./wolf3d map/ground      # Niveau 2
./wolf3d map/underground # Niveau 3
```

### 🕹️ Contrôles

#### ⌨️ Clavier

| Touche | Action |
|--------|--------|
| **Z** / **W** | ⬆️ Avancer |
| **S** | ⬇️ Reculer |
| **Q** / **A** | ⬅️ Aller à gauche |
| **D** / **E** | ➡️ Aller à droite |
| **V** | 🏃 Activer/Désactiver le sprint |
| **L** | 💡 Allumer/Éteindre la lampe torche |
| **ESC** | 🏠 Retour au menu |
| **←** / **→** | 🔄 Tourner (alternatif à la souris) |

#### 🖱️ Souris

| Action | Effet |
|--------|-------|
| **Bouger la souris** | 👀 Regarder autour |
| **Clic gauche** | 🔫 Tirer |

### 💡 Conseils de jeu

- 🎯 **Visez bien** : Les ennemis sont mobiles et réactifs
- 💊 **Gérez votre santé** : Elle ne se régénère pas automatiquement
- 🔦 **Utilisez la lampe** : Les Backrooms sont sombres et labyrinthiques
- 🏃 **Sprint tactique** : Utilisez-le pour fuir ou esquiver
- 💾 **Sauvegarde auto** : Le jeu sauvegarde à chaque fin de niveau
- 🏚️ **Explorez** : Les Backrooms cachent des secrets

## 🗂️ Structure du projet

```
wolf3d/
├── 📁 src/           # Code source
├── 📁 include/       # Fichiers d'en-tête
├── 📁 map/           # Fichiers de cartes
├── 📁 ressources/    # Assets (textures, sons, sprites)
├── 📄 Makefile       # Fichier de compilation
└── 📄 README.md      # Ce fichier
```

## 🛠️ Résolution de problèmes

### ❌ Le jeu ne se lance pas

```bash
# Vérifier que CSFML est installé
ldconfig -p | grep csfml

# Si non installé, réinstaller
sudo apt-get install libcsfml-dev
```

### ❌ Erreur de compilation

```bash
# Nettoyer et recompiler
make fclean
make
```

### ❌ Problème de performance

- 🖥️ Réduire la résolution dans les paramètres
- 💡 Désactiver la lampe torche si non nécessaire
- 🔄 Fermer les autres applications

## 👥 Auteurs

Ce projet a été réalisé dans le cadre du cursus **Epitech** par :

- 👨‍💻 **Deniz DEMIR**
- 👩‍💻 **Alycia ASLI**
- 👩‍💻 **Donata CONTANT**

## 📜 Licence

Ce projet est réalisé dans le cadre pédagogique d'Epitech. Toute réutilisation est soumise aux conditions du cursus Epitech.

---

<div align="center">

**🎮 Bon jeu ! 🎮**

*Projet réalisé en 2025*

</div>
