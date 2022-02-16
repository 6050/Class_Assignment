SELECT name AS '체육관 관장을 맡고 있지 않은 트레이너의 이름'
FROM Trainer
WHERE id NOT IN(SELECT leader_id FROM Gym)
ORDER BY name;