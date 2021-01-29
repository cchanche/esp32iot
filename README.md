<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/cchanche/esp32iot">
    <img src="images/enseeiht.jpeg" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Projet IoT 3A IBDIoT</h3>
</p>

## A propos du projet

L'idée principale de ce projet était d'utiliser un microcontrôleur ESP32 pour l'envoi de mails.

## Client mail

L'objectif était, tout d'abord, de construire un mail de toute pièce, c'est-à-dire spécifier un ou plusieurs destinataires, un objet, le contenu textuel ainsi qu'une ou des pièces jointes à partir d'un script C++. Par la suite, le microcontrôleur, connecté en WIFI au point d'accès d'un de nos smartphones, pouvait se connecter auprès du serveur SMTP de gmail et donc envoyer ce précédent mail en tant que client mail.

## Serveur Web

A la suite de cela, nous avons décidé d'ajouter une certaine flexibilité au corps du mail que l'on envoyait. En effet, l'idée était de laisser le choix à l'utilisateur du corps du mail. Pour cela, nous avons utilisé notre carte ESP32 en tant que serveur web. Chaque requête de l'utilisateur vers notre carte ESP32 (à l'aide d'un quelconque navigateur) engendrait une réponse contenant une page web simple permettant de remplir les différentes parties du mail (citées précédemment). Par la suite, notre serveur web "ESP32" pouvait récupérer et envoyer le mail de la même manière que dans la partie précédente.

## Communication LoRa

Pour finir, la dernière fonctionnalité implémentée sur notre microcontrôleur ESP32 était la communication pair-à-pair à l'aide du protocole LoRaWAN avec un autre microcontrôleur du même type. L'objectif était, ici, de se placer dans un cas d'utilisation. L'objectif était de prévoir la situation dans laquelle notre microcontrôleur se trouverait en extérieur ou du moins dans une zone non couverte par le WIFI et qu'il voudrait envoyer un signal/une alerte par mail tout de même (d'où l'utilisation d'un protocole longue distance tel que LoRaWAN). Il fallait donc mettre en place cette communication entre les deux microcontrôleurs, puis, s'inspirer grandement de la section précédente pour permettre à celui ayant accès au WIFI d'envoyer le mail.

## Pour aller plus loin : capteurs et applications

## Cloner le projet

### Prérequis

This is an example of how to list things you need to use the software and how to install them.

- npm
  ```sh
  npm install npm@latest -g
  ```

### Installation

1. Get a free API Key at [https://example.com](https://example.com)
2. Clone the repo
   ```sh
   git clone https://github.com/your_username_/Project-Name.git
   ```
3. Install NPM packages
   ```sh
   npm install
   ```
4. Enter your API in `config.js`
   ```JS
   const API_KEY = 'ENTER YOUR API';
   ```

<!-- USAGE EXAMPLES -->

## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_
