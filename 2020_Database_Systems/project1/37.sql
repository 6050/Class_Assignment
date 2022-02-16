SELECT name AS '잡은 포켓몬의 레벨의 총합이 가장 높은 트레이너의 이름', SUM(level) AS '레벨의 총합'
FROM Trainer AS T, CatchedPokemon
WHERE T.id = owner_id
GROUP BY name
HAVING COUNT(*) >= ALL(SELECT COUNT(*)
                       FROM Trainer AS T, CatchedPokemon
                       WHERE T.id = owner_id
                       GROUP BY name)
ORDER BY name;