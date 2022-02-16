SELECT T.id AS '포켓몬을 가장 많이 잡은 트레이너의 ID', COUNT(*) AS '잡은 포켓몬의 수' 
FROM Trainer AS T, CatchedPokemon
WHERE T.id = owner_id
GROUP BY T.id
HAVING COUNT(*) >= ALL(SELECT COUNT(*)
                       FROM Trainer AS T, CatchedPokemon
                       WHERE T.id = owner_id
                       GROUP BY T.id)
ORDER BY T.id;