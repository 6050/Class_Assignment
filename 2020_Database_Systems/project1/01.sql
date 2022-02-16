SELECT name AS '잡은 포켓몬이 3마리 이상인 트레이너의 이름'
FROM Trainer AS T, CatchedPokemon
WHERE T.id = owner_id
GROUP BY name
HAVING COUNT(*) >= 3
ORDER BY COUNT(*) DESC;