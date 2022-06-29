<h1> Projet SLV </h1>
  
  <h3> Ecole Nationale des Ponts et Chaussées x Master MFD </h3>
  
  Ismaël BADAROU - Victor BELLISSANT - Raphaël LASRY - Ludovic QUILY
  
  Le code est structuré en divers fichiers .cpp/.h et contient plusieurs classes. En particulier, on a défini un modèle d'Heston ainsi qu'un simulateur de scénarii Monte-Carlo. D'autre part, nous avons repris le projet de Décembre permettant de calculer la volatilité implicite du modèle de Dupire. Enfin, nous avons calculé l'espérance conditionnelle :
  $$ \mathbb{E}\left[V_{t_i} \mid S_{t_i} = s_{i,j} \right] $$
  Via une approximation détaillée dans ce papier : https://www.researchgate.net/publication/272219461_The_Heston_Stochastic-Local_Volatility_Model_Efficient_Monte_Carlo_Simulation
  
  Le main permet de générer un prix de marché artificiel via un modèle d'Heston, une fonction de Newton-Raphson permet d'inverser ce modèle pour en obtenir la volatilité implicite du modèle de Black-Scholes que l'on bruite. Enfin, on génère des prix via le pricer SLV, et on compare les résultats des deux modèles.
